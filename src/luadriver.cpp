#include "luadriver.hpp"
#include "building.hpp"
#include "globalutilities.hpp"
#include "generationSettings.hpp"
#include "biome.hpp"
#include "types.hpp"

#include <SFML/System/Clock.hpp>
#include <filesystem>
#include <iostream>
#include <cmath>

namespace lua {
// Resources requested to be loaded by Lua scripts.
std::vector <core::TextureData> load_queue;

namespace driver {
    std::shared_ptr <Driver> Driver::instance = nullptr;

    Driver::Driver() {
        this->L = luaL_newstate();
        luaL_openlibs(this->L);

        this->registerLua();
    }

    Driver::~Driver() {
        lua_close(this->L);
    }

    Driver* Driver::getInstance() {
        if(instance.get() == nullptr)
            instance = std::shared_ptr <Driver> (new Driver());
        return instance.get();
    } 

    void Driver::loadGameData() {
        sf::Clock clock;
        
        // Read data from directories.
        namespace fs = std::filesystem;

        // Read world generation data.
        {
            std::string generationdata_path = "./data/world_generation.lua";
            std::cout << "[Lua Driver]: Reading world generation config.\n";
            luaL_dofile(this->L, generationdata_path.c_str());

            auto worldconfig = luabridge::getGlobal(this->L, "GameSettings");

            iso::GameSettings settings;
            settings.region_persistence          = worldconfig["region_persistence"];
            settings.world_persistence           = worldconfig["world_persistence"];
            settings.simulation_update_frequency = worldconfig["simulation_update_frequency"];
            settings.world_width                 = worldconfig["World"]["size"];
            settings.terrain_from                = worldconfig["World"]["terrain_from"];
            settings.forest_from                 = worldconfig["World"]["forest_from"];
            settings.fog_of_war_enabled          = worldconfig["World"]["fog_of_war_enabled"];
            settings.temperature_multiplier      = worldconfig["World"]["multiplier_temperature"];
            settings.moisture_multiplier         = worldconfig["World"]["multiplier_moisture"];
            settings.region_width                = worldconfig["Region"]["size"];
            settings.initial_population          = worldconfig["Region"]["initial_population"];
            settings.population_needs_enabled    = worldconfig["Region"]["population_needs_enabled"];
            settings.building_cost_enabled       = worldconfig["Region"]["building_cost_enabled"];
            settings.astar_pathfinding_enabled   = worldconfig["Region"]["astar_pathfinding_enabled"];

            game_settings = settings;
        }

        // Read item data.
        {
            std::string resourcedata_path = "./data/item/";
            std::cout << "[Lua Driver]: Loading items.\n";
            for(const auto& file : fs::directory_iterator(resourcedata_path)) {
                const auto& file_path = file.path().string();
                const auto& file_path_extension = file.path().extension().string();

                if(file_path_extension == ".lua") {
                    std::cout << "[Lua Driver]: Registering item '" << file_path << "'.\n";
                    luaL_dofile(this->L, file_path.c_str());

                    auto itemdata = luabridge::getGlobal(this->L, "Item");

                    iso::ItemData data;
                    data.filename     = file_path;
                    data.name         = this->readString(itemdata["name"]);
                    data.description  = this->readString(itemdata["description"]);
                    data.icon_texture = this->readString(itemdata["icon_texture"]);
                    data.icon_size    = this->readVector2i(itemdata["icon_size"]);
                    data.type         = this->readString(itemdata["type"]);
                    data.amount       = 0;
                
                    iso::StorageItem item(data);
                    items.push_back(item);
                }
            }   
        }

        // Read resource data.
        {
            std::string resourcedata_path = "./data/resource/";
            std::cout << "[Lua Driver]: Loading resources.\n";
            for(const auto& file : fs::directory_iterator(resourcedata_path)) {
                const auto& file_path = file.path().string();
                const auto& file_path_extension = file.path().extension().string();

                if(file_path_extension == ".lua") {
                    std::cout << "[Lua Driver]: Registering resource '" << file_path << "'.\n";
                    luaL_dofile(this->L, file_path.c_str());

                    auto resourcedata = luabridge::getGlobal(this->L, "Resource");

                    iso::ResourceData data;
                    data.filename            = file_path;
                    data.name                = this->readString(resourcedata["name"]);
                    data.description         = this->readString(resourcedata["description"]);
                    data.texture             = this->readString(resourcedata["texture"]);
                    data.texture_size        = this->readVector2i(resourcedata["texture_size"]);
                    data.icon_texture        = this->readString(resourcedata["icon_texture"]);
                    data.icon_size           = this->readVector2i(resourcedata["icon_size"]);
                    data.type                = this->readString(resourcedata["type"]);
                    data.min_occurence       = this->readInteger(resourcedata["minimum_occurence"]);
                    data.max_occurence       = this->readInteger(resourcedata["maximum_occurence"]);
                    data.chance              = this->readNumber(resourcedata["generation_chance"]);
                    data.patch_size          = this->readInteger(resourcedata["patch_size"]);
                    data.tile_requirements   = this->readVectorString(resourcedata["tile"]);
                    data.region_requirements = this->readVectorString(resourcedata["region"]); 

                    iso::Resource resource(data);
                    resources.push_back(resource);   
                }
            }
        }

        // Read building data.
        {
            std::string buildingdata_path = "./data/building/";
            std::cout << "[Lua Driver]: Loading buildings.\n";
            for(const auto& file : fs::directory_iterator(buildingdata_path)) {
                const auto& file_path = file.path().string();                        // Whole filename.
                const auto& file_path_extension = file.path().extension().string();  // Just the extension.     

                if(file_path_extension == ".lua") {
                    std::cout << "[Lua Driver]: Registering building '" << file_path << "'.\n";
                    luaL_dofile(this->L, file_path.c_str()); 

                    auto buildingdata = luabridge::getGlobal(this->L, "Building");

                    iso::BuildingData data;
                    data.filename     = file_path;                
                    data.name         = this->readString(buildingdata["name"]);
                    data.description  = this->readString(buildingdata["description"]);
                    data.size         = this->readVector2i(buildingdata["size"]);
                    data.texture      = this->readString(buildingdata["texture"]);
                    data.texture_size = this->readVector2i(buildingdata["texture_size"]);
                    data.icon_texture = this->readString(buildingdata["icon_texture"]);
                    data.icon_size    = this->readVector2i(buildingdata["icon_size"]);
                    data.removable    = this->readBoolean(buildingdata["removable"]);
                    data.scan_area    = this->readVector2i(buildingdata["scan_area"]);
                    data.harvests     = this->getHarvestedResourceList(data.filename);
                    data.produces     = this->getProducedResourceList(data.filename);

                    iso::Building building(data);
                    buildings.push_back(building);
                }
            }
        }

        // Read biome data.
        {
            std::string biomedata_path = "./data/biome/";
            std::cout << "[Lua Driver]: Loading biomes.\n";
            for(const auto& file : fs::directory_iterator(biomedata_path)) {
                const auto& file_path = file.path().string();                        // Whole filename.
                const auto& file_path_extension = file.path().extension().string();  // Just the extension.     

                if(file_path_extension == ".lua") {
                    std::cout << "[Lua Driver]: Registering biome '" << file_path << "'.\n";
                    luaL_dofile(this->L, file_path.c_str()); 

                    auto biomedata = luabridge::getGlobal(this->L, "Biome");

                    iso::BiomeData data;
                    data.filename                = file_path;
                    data.name                    = this->readString(biomedata["name"]);
                    data.id                      = this->readString(biomedata["id"]);
                    data.description             = this->readString(biomedata["description"]);
                    data.colour_wmap             = this->readColour(biomedata["colour_wmap"]);
                    data.temperature             = this->readString(biomedata["temperature"]);
                    data.moisture                = this->readString(biomedata["moisture"]);
                    data.can_be_forest           = this->readBoolean(biomedata["can_be_forest"]);
                    data.forest_texture_worldmap = this->readString(biomedata["forest_texture_worldmap"]);
                    data.tiles                   = this->readVectorString(biomedata["tiles"]);
                    data.trees                   = this->readVectorString(biomedata["trees"]);

                    iso::Biome biome(data);
                    biomes.push_back(biome);
                }
            } 
        }
    
        const float time_rounded = std::ceil(clock.getElapsedTime().asSeconds() * 100) / 100;
        std::cout << "[Lua Driver]: Loaded data in " << time_rounded << "s.\n";
    }

    void Driver::loadResources(iso::SimulationManager* manager, const std::string& filename) {
        std::cout << "====================\n";
        std::cout << "[] Loading resources from file '" << filename << "'\n";
        load_queue.resize(0);

        sf::Clock clock;
        luaL_dofile(this->L, filename.c_str());
    
        for(auto data : load_queue) {
            if(manager->resource.checkTextureExists(data.id))
                continue;
            
            manager->resource.loadTexture(data.filename, data.id, sf::IntRect(data.position.asSFMLVector2i(), data.size.asSFMLVector2i()));
        }

        const float time_rounded = std::ceil(clock.getElapsedTime().asSeconds() * 100) / 100;
        std::cout << "[] Loaded resources in " << time_rounded << "s.\n";
        std::cout << "====================\n";
    }

    // Lua wrapper functions.

    std::vector <iso::BuildingHarvest> Driver::getHarvestedResourceList(const std::string& filename) const {
        luaL_dofile(this->L, filename.c_str());
        
        std::vector <iso::BuildingHarvest> list;
        int result_global = lua_getglobal(this->L, "Building");
        if(result_global == LUA_TNIL)
            return list; 

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Could not get table 'Building' from " << filename << "\n";
            return list; 
        }

        lua_pushstring(this->L, "harvests");
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return list;

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: There is no 'harvests' table.\n";
            return list; 
        }

        // Get length of the 'harvests' array.
        int array_length = lua_rawlen(this->L, -1); 

        for(int i = 0; i < array_length; i++) {
            iso::BuildingHarvest harvest;

            // Lua indexes start at 1, and not 0.
            int lua_index = i + 1;
            
            lua_rawgeti(this->L, -1, lua_index);
            lua_pushstring(this->L, "name");
            lua_gettable(this->L, -2);
            harvest.name = lua_tostring(this->L, -1);
            
            // Pop 'name' from stack.
            lua_pop(this->L, 1);

            // Read the 'harvest' array.

            lua_pushstring(this->L, "harvest");
            lua_gettable(this->L, -2);    

            for(const auto& resource : resources) {
                lua_pushstring(this->L, resource.getResourceName().c_str());
                int resource_exists = lua_gettable(this->L, -2);

                if(resource_exists != LUA_TNIL) {
                    int quantity = lua_tointeger(this->L, -1);
                    
                    iso::StorageItem item;
                    item = resource;
                    item.setAmount(quantity);

                    harvest.harvest.push_back(item);
                }

                // Pop resource name from stack.
                lua_pop(this->L, 1);
            }

            list.push_back(harvest);

            // Pop 'harvest' the array from stack.
            lua_pop(this->L, 2);
        }

        // Pop 'harvests' and 'Building' from the stack.
        lua_pop(this->L, 2);
        return list;
    }

    std::vector <iso::BuildingProduction> Driver::getProducedResourceList(const std::string& filename) const {
        luaL_dofile(this->L, filename.c_str());
        
        std::vector <iso::BuildingProduction> list;
        int result_global = lua_getglobal(this->L, "Building");
        if(result_global == LUA_TNIL)
            return list;

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Could not get table 'Building' from " << filename << "\n";
            return list; 
        }

        lua_pushstring(this->L, "produces");
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return list;
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: There is no 'produces' table.\n";
            return list; 
        }

        // Get length of 'produces'
        int array_length = lua_rawlen(this->L, -1);

        for(int i = 0; i < array_length; i++) {
            iso::BuildingProduction production;

            // Lua indexes start at 1, and not 0.
            int lua_index = i + 1;

            lua_rawgeti(this->L, -1, lua_index);
            
            lua_pushstring(this->L, "name");
            lua_gettable(this->L, -2);
            production.name = lua_tostring(this->L, -1);

            // Pop 'name' from stack.
            lua_pop(this->L, 1);

            lua_pushstring(this->L, "amount");
            lua_gettable(this->L, -2);
            production.amount = lua_tointeger(this->L, -1);

            // Pop 'amount' from stack.
            lua_pop(this->L, 1);

            // Read the 'requires' array.

            lua_pushstring(this->L, "requires");
            lua_gettable(this->L, -2);

            for(const auto& resource : resources) {
                lua_pushstring(this->L, resource.getResourceName().c_str());
                int resource_exists = lua_gettable(this->L, -2);

                if(resource_exists != LUA_TNIL) {
                    int quantity = lua_tointeger(this->L, -1);

                    iso::StorageItem item;
                    item = resource;
                    item.setAmount(quantity);

                    production.requirements.push_back(item);
                }

                // Pop resource name from stack.
                lua_pop(this->L, 1);
            }

            list.push_back(production);

            // Pop 'requires' and the array from stack.
            lua_pop(this->L, 2);
        }

        // Pop 'produces' and 'Building' from the stack.
        lua_pop(this->L, 2);
        return list;
    }

    std::vector <std::string> Driver::getRegionRequirements(const std::string& filename) const {
        luaL_dofile(this->L, filename.c_str());

        std::vector <std::string> list;
        int result_global = lua_getglobal(this->L, "Resource");
        if(result_global == LUA_TNIL)
            return list;
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Could not get table 'Resource' from " << filename << "\n";
            return list;
        }

        lua_pushstring(this->L, "region");
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return list;
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: There is no 'region' table.\n";
            return list;
        }

        // Get length of 'region'
        int array_length = lua_rawlen(this->L, -1);

        for(int i = 0; i < array_length; i++) {
            // Lua indexes start at 1, and not 0.
            int lua_index = i + 1;

            lua_rawgeti(this->L, -1, lua_index);
            std::string region_property = lua_tostring(this->L, -1);
            list.push_back(region_property);

            // Pop the pushed index from stack.
            lua_pop(this->L, 1); 
        }

        // Pop 'region' and 'Resource' from the stack.
        lua_pop(this->L, 2);
        return list;
    }

    std::vector <std::string> Driver::getTileRequirements(const std::string& filename) const {
        luaL_dofile(this->L, filename.c_str());

        std::vector <std::string> list;
        int result_global = lua_getglobal(this->L, "Resource");
        if(result_global == LUA_TNIL)
            return list;
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Could not get table 'Resource' from " << filename << "\n";
            return list;
        }

        lua_pushstring(this->L, "tile");
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return list;
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: There is no 'tile' table.\n";
            return list;
        }

        // Get length of 'tile'
        int array_length = lua_rawlen(this->L, -1);

        for(int i = 0; i < array_length; i++) {
            // Lua indexes start at 1, and not 0.
            int lua_index = i + 1;

            lua_rawgeti(this->L, -1, lua_index);
            std::string region_property = lua_tostring(this->L, -1);
            list.push_back(region_property);

            // Pop the pushed index from stack.
            lua_pop(this->L, 1); 
        }

        // Pop 'tile' and 'Resource' from the stack.
        lua_pop(this->L, 2);
        return list;
    }

    void Driver::stackSize() const {
        std::cout << "[Lua Driver] Stack size is: " << lua_gettop(this->L) << "\n";
    }

    void Driver::stackDump() const {
        std::cout << "=====[Lua Stack Dump]=====\n";

        int top = lua_gettop(this->L);
        for(int i = 1; i <= top; i++) {
            std::cout << "[" << i << "][" << iso::capitalise(lua_typename(this->L, i)) << "]: ";

            switch(lua_type(this->L, i)) {
                case LUA_TNUMBER: 
                    std::cout << lua_tonumber(this->L, i);
                    break;
                case LUA_TSTRING:
                    std::cout << lua_tostring(this->L, i);
                    break;
                case LUA_TBOOLEAN:
                    std::cout << lua_toboolean(this->L, i);
                    break;
                case LUA_TNIL:
                    std::cout << "Nil";
                    break;
                case LUA_TTABLE:
                    std::cout << "Table";
                    break;
            }

            std::cout << "\n";
        }

        std::cout << "==========================\n";
    }

    std::vector <std::string> Driver::readBiomeTiles(const std::string& filename) const {
        luaL_dofile(this->L, filename.c_str());

        std::vector <std::string> list;
        int result_global = lua_getglobal(this->L, "Biome");
        if(result_global == LUA_TNIL)
            return list;
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Could not get table 'Biome' from " << filename << "\n";
            return list;
        }

        lua_pushstring(this->L, "tiles");
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return list;
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: There is no 'tiles' table.\n";
            return list;
        }

        // Get length of 'tiles'
        int array_length = lua_rawlen(this->L, -1);

        for(int i = 0; i < array_length; i++) {
            // Lua indexes start at 1, and not 0.
            int lua_index = i + 1;

            lua_rawgeti(this->L, -1, lua_index);
            std::string tile_texture = lua_tostring(this->L, -1);
            list.push_back(tile_texture);

            // Pop texture from stack.
            lua_pop(this->L, 1);
        }

        // Pop 'tiles' and 'Biome' from the stack.
        lua_pop(this->L, 2);
        return list;
    }

    std::vector <std::string> Driver::readBiomeTrees(const std::string& filename) const {
        luaL_dofile(this->L, filename.c_str());

        std::vector <std::string> list;
        int result_global = lua_getglobal(this->L, "Biome");
        if(result_global == LUA_TNIL)
            return list;
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Could not get table 'Biome' from " << filename << "\n";
            return list;
        }

        lua_pushstring(this->L, "trees");
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return list;
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: There is no 'trees' table.\n";
            return list;
        }

        // Get length of 'trees'
        int array_length = lua_rawlen(this->L, -1);

        for(int i = 0; i < array_length; i++) {
            // Lua indexes start at 1, and not 0.
            int lua_index = i + 1;

            lua_rawgeti(this->L, -1, lua_index);
            std::string tile_texture = lua_tostring(this->L, -1);
            list.push_back(tile_texture);

            // Pop texture from stack.
            lua_pop(this->L, 1);
        }

        // Pop 'trees' and 'Biome' from the stack.
        lua_pop(this->L, 2);
        return list;
    }

    void Driver::registerLua() const {
        luabridge::getGlobalNamespace(this->L)
            .beginClass <core::Colour> ("Colour")
                .addConstructor <void (*) (uint8_t, uint8_t, uint8_t, uint8_t)> ()
                .addProperty("r", core::Colour::getR, core::Colour::setR)
                .addProperty("g", core::Colour::getG, core::Colour::setG)
                .addProperty("b", core::Colour::getB, core::Colour::setB)
                .addProperty("a", core::Colour::getA, core::Colour::setA)
            .endClass()
            .beginClass <core::Vector2f> ("Vector2f")
                .addConstructor <void (*) (float, float)> ()
                .addProperty("x", core::Vector2f::getX, core::Vector2f::setX)
                .addProperty("y", core::Vector2f::getY, core::Vector2f::setY)
            .endClass()
            .beginClass <core::Vector2i> ("Vector2i")
                .addConstructor <void (*) (int, int)> ()
                .addProperty("x", core::Vector2i::getX, core::Vector2i::setX)
                .addProperty("y", core::Vector2i::getY, core::Vector2i::setY)
            .endClass()
            .beginClass <core::Vector3f> ("Vector3f")
                .addConstructor <void (*) (float, float, float)> ()
                .addProperty("x", core::Vector3f::getX, core::Vector3f::setX)
                .addProperty("y", core::Vector3f::getY, core::Vector3f::setY)
                .addProperty("z", core::Vector3f::getZ, core::Vector3f::setZ)
            .endClass()
            .beginClass <core::Vector3i> ("Vector3i")
                .addConstructor <void (*) (int, int, int)> ()
                .addProperty("x", core::Vector3i::getX, core::Vector3i::setX)
                .addProperty("y", core::Vector3i::getY, core::Vector3i::setY)
                .addProperty("z", core::Vector3i::getZ, core::Vector3i::setZ)
            .endClass()
            .beginClass <core::TextureData> ("TextureData")
            .endClass()
            .addFunction("loadTexture", &lua::loadTexture);
    }

    std::vector <std::string> Driver::readVectorString(luabridge::LuaRef reference) const {
        std::vector <std::string> list;
        if(reference.isNil())
            return list;

        int length = reference.length();
        for(int i = 1; i <= length; i++) {
            list.push_back(reference[i]);
        }

        return list;
    }

    bool Driver::readBoolean(luabridge::LuaRef reference) const {
        if(reference.isNil())
            return false;
        return (bool)reference;
    } 

    std::string Driver::readString(luabridge::LuaRef reference) const {
        if(reference.isNil())
            return std::string();
        return (std::string)reference;
    }

    int Driver::readInteger(luabridge::LuaRef reference) const {
        if(reference.isNil())
            return 0;
        return (int)reference;
    }

    float Driver::readNumber(luabridge::LuaRef reference) const {
        if(reference.isNil())
            return 0.0f;
        return (float)reference;
    }

    core::Vector2i Driver::readVector2i(luabridge::LuaRef reference) const {
        if(reference.isNil())
            return core::Vector2i();
        return (core::Vector2i)reference;
    }

    core::Colour Driver::readColour(luabridge::LuaRef reference) const {
        if(reference.isNil())
            return core::Colour();
        return (core::Colour)reference;
    }
}

void loadTexture(const std::string& filename, const std::string& id, core::Vector2i position, core::Vector2i size) {
    core::TextureData data;
    data.filename = filename;
    data.id       = id;
    data.position = position;
    data.size     = size;
    load_queue.push_back(data);
}
}
    