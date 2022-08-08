#include "luadriver.hpp"
#include "building.hpp"
#include "globalutilities.hpp"
#include "generationSettings.hpp"

#include "tileType.hpp"

#include <filesystem>
#include <iostream>

namespace lua {
namespace driver {
    Driver::Driver() {
        this->L = luaL_newstate();

        // Read data from directories.
        namespace fs = std::filesystem;

        // Read application config.
        {
            std::string config_path = "./data/appconfig.lua";
            std::cout << "[Lua Driver]: Reading application config.\n";
            luaL_dofile(this->L, config_path.c_str());

            iso::Settings settings;
            settings.window_size            = this->getNestedFieldValueVector2i("Appconfig", "window_size", "x", "y");
            settings.window_fullscreen      = this->getFieldValueBoolean("Appconfig", "fullscreen");
            settings.window_vsync           = this->getFieldValueBoolean("Appconfig", "vsync");
            settings.window_refresh_rate    = this->getFieldValueInteger("Appconfig", "refresh_rate");
            settings.application_debug_mode = this->getFieldValueBoolean("Appconfig", "debug_mode");
        }

        // Read world generation data.
        {
            std::string generationdata_path = "./data/world_generation.lua";
            std::cout << "[Lua Driver]: Reading world generation config.\n";
            luaL_dofile(this->L, generationdata_path.c_str());

            iso::GameSettings settings;
            settings.persistence                 = this->getFieldValueInteger("GameSettings", "persistence");
            settings.simulation_update_frequency = this->getFieldValueFloat("GameSettings", "simulation_update_frequency");
            settings.world_width                 = this->getNestedFieldValueInteger("GameSettings", "World", "size");
            settings.terrain_from                = this->getNestedFieldValueFloat("GameSettings", "World", "terrain_from");
            settings.forest_from                 = this->getNestedFieldValueFloat("GameSettings", "World", "forest_from");
            settings.fog_of_war_enabled          = this->getNestedFieldValueBoolean("GameSettings", "World", "fog_of_war_enabled");
            settings.region_width                = this->getNestedFieldValueInteger("GameSettings", "Region", "size");
            settings.initial_population          = this->getNestedFieldValueInteger("GameSettings", "Region", "initial_population");
            settings.population_needs_enabled    = this->getNestedFieldValueBoolean("GameSettings", "Region", "population_needs_enabled");
            settings.building_cost_enabled       = this->getNestedFieldValueBoolean("GameSettings", "Region", "building_cost_enabled");
            settings.astar_pathfinding_enabled   = this->getNestedFieldValueBoolean("GameSettings", "Region", "astar_pathfinding_enabled");
    
            game_settings = settings;
        }

        // Read item data.
        {
            std::string resourcedata_path = "./data/resource/";
            std::cout << "[Lua Driver]: Loading items.\n";
            for(const auto& file : fs::directory_iterator(resourcedata_path)) {
                const auto& file_path = file.path().string();
                const auto& file_path_extension = file.path().extension().string();

                if(file_path_extension == ".lua") {
                    std::cout << "[Lua Driver]: Registering resource '" << file_path << "'.\n";
                    luaL_dofile(this->L, file_path.c_str());

                    iso::ItemData data;
                    data.filename = file_path;
                    data.name         = this->getFieldValueString("Resource", "name");
                    data.description  = this->getFieldValueString("Resource", "description");
                    data.icon_texture = this->getFieldValueString("Resource", "icon_texture");
                    data.icon_size    = this->getNestedFieldValueVector2i("Resource", "icon_size", "x", "y");
                    data.type         = this->getFieldValueString("Resource", "type");
                    data.amount       = 0;
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

                    iso::ResourceData data;
                    data.filename      = file_path;
                    data.name          = this->getFieldValueString("Resource", "name");
                    data.description   = this->getFieldValueString("Resource", "description");
                    data.texture       = this->getFieldValueString("Resource", "texture");
                    data.texture_size  = this->getNestedFieldValueVector2i("Resource", "texture_size", "x", "y");
                    data.icon_texture  = this->getFieldValueString("Resource", "icon_texture");
                    data.icon_size     = this->getNestedFieldValueVector2i("Resource", "icon_size", "x", "y");
                    data.type          = this->getFieldValueString("Resource", "type");
                    data.min_occurence = this->getFieldValueInteger("Resource", "minimum_occurence");
                    data.max_occurence = this->getFieldValueInteger("Resource", "maximum_occurence");
                    data.chance        = this->getFieldValueFloat("Resource", "generation_chance");
                    data.patch_size    = this->getFieldValueInteger("Resource", "patch_size");
                    data.tile_requirements   = this->getTileRequirements(data.filename);
                    data.region_requirements = this->getRegionRequirements(data.filename); 

                    iso::Resource resource(data);
                    resources.push_back(resource);   
                }
            }
        }

        // Read buildings data.
        {
            std::string buildingdata_path = "./data/building/";
            std::cout << "[Lua Driver]: Loading buildings.\n";
            for(const auto& file : fs::directory_iterator(buildingdata_path)) {
                const auto& file_path = file.path().string();                        // Whole filename.
                const auto& file_path_extension = file.path().extension().string();  // Just the extension.     

                if(file_path_extension == ".lua") {
                    std::cout << "[Lua Driver]: Registering building '" << file_path << "'.\n";
                    luaL_dofile(this->L, file_path.c_str()); 

                    iso::BuildingData data;
                    data.filename     = file_path;                
                    data.name         = this->getFieldValueString("Building", "name");
                    data.description  = this->getFieldValueString("Building", "description");
                    data.size         = this->getNestedFieldValueVector2i("Building", "size", "x", "y");
                    data.texture      = this->getFieldValueString("Building", "texture");
                    data.texture_size = this->getNestedFieldValueVector2i("Building", "texture_size", "x", "y");
                    data.icon_texture = this->getFieldValueString("Building", "icon_texture");
                    data.icon_size    = this->getNestedFieldValueVector2i("Building", "icon_size", "x", "y");
                    data.removable    = this->getFieldValueBoolean("Building", "removable");
                    data.scan_area    = this->getNestedFieldValueVector2i("Building", "scan_area", "x", "y"); 
                    data.harvests     = this->getHarvestedResourceList(data.filename);
                    data.produces     = this->getProducedResourceList(data.filename);

                    iso::Building building(data);
                    buildings.push_back(building);
                }
            }
        }
    }

    Driver::~Driver() {
        lua_close(L);
    }

    // Lua wrapper functions.

    std::string Driver::getFieldValueString(const std::string& field, const std::string& key) const {
        int result_global = lua_getglobal(this->L, field.c_str());
        if(result_global == LUA_TNIL)
            return "";

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field << " is not a table.\n";
            return "";
        }

        lua_pushstring(this->L, key.c_str());
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return "";

        if(!lua_isstring(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Value at " << key << " can not be stringified.\n";
            return ""; 
        }

        std::string value = lua_tostring(this->L, -1);
        lua_pop(this->L, 2);

        return value;
    }

    int Driver::getFieldValueInteger(const std::string& field, const std::string& key) const {
        int result_global = lua_getglobal(this->L, field.c_str());
        if(result_global == LUA_TNIL)
            return 0;

        if(!lua_istable(this->L, -1)) {
            std::cout  << "[Lua Driver Error]: " << field << " is not a table.\n";
            return 0;
        }

        lua_pushstring(this->L, key.c_str());
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return 0;

        if(!lua_isinteger(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Value at " << key << " is not a integer.\n";
            return 0; 
        }

        int value = lua_tointeger(this->L, -1);
        lua_pop(this->L, 2);

        return value;
    } 

    float Driver::getFieldValueFloat(const std::string& field, const std::string& key) const {
        int result_global = lua_getglobal(this->L, field.c_str());
        if(result_global == LUA_TNIL)
            return 0.0f;

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field << " is not a table.\n";
            return 0.0f;
        }

        lua_pushstring(this->L, key.c_str());
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return 0.0f;

        if(!lua_isnumber(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Value at " << key << " is not a number.\n";
            return 0.0f; 
        }

        float value = (float)lua_tonumber(this->L, -1);
        lua_pop(this->L, 2);

        return value;
    } 

    bool Driver::getFieldValueBoolean(const std::string& field, const std::string& key) const {
        int result_global = lua_getglobal(this->L, field.c_str());
        if(result_global == LUA_TNIL)
            return false;

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field << " is not a table.\n";
            return false;
        }

        lua_pushstring(this->L, key.c_str());
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return false;

        if(!lua_isboolean(this->L, -1)) {
            std::cout << "[Lua Driver Error]: Value at " << key << " is not a boolean.\n";
            return false; 
        }

        bool value = lua_toboolean(this->L, -1);
        lua_pop(this->L, 2);

        return value;
    }

    sf::Vector2i Driver::getFieldValueVector2i(const std::string& field, const std::string& key1, const std::string& key2) const {
        sf::Vector2i result;
        result.x = this->getFieldValueInteger(field, key1);
        result.y = this->getFieldValueInteger(field, key2);
        return result;
    }

    std::string Driver::getNestedFieldValueString(const std::string& field1, const std::string& field2, const std::string& key) const {
        int result_global = lua_getglobal(this->L, field1.c_str());
        if(result_global == LUA_TNIL)
            return "";
    
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field1 << " is not a table.\n";
            return "";
        }

        lua_pushstring(this->L, field2.c_str());
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return "";

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field2 << " is not a table.\n";
            return "";
        }
        
        lua_pushstring(this->L, key.c_str());
        lua_gettable(this->L, -2);
        std::string result = lua_tostring(this->L, -1);
        lua_pop(this->L, 1);
        
        lua_pop(this->L, 2);
        return result;
    }

    int Driver::getNestedFieldValueInteger(const std::string& field1, const std::string& field2, const std::string& key) const {
        int result_global = lua_getglobal(this->L, field1.c_str());
        if(result_global == LUA_TNIL)
            return 0;
    
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field1 << " is not a table.\n";
            return 0;
        }

        lua_pushstring(this->L, field2.c_str());
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return 0;

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field2 << " is not a table.\n";
            return 0;
        }
        
        lua_pushstring(this->L, key.c_str());
        lua_gettable(this->L, -2);
        int result = lua_tointeger(this->L, -1);
        lua_pop(this->L, 1);
        
        lua_pop(this->L, 2);
        return result;
    }

    float Driver::getNestedFieldValueFloat(const std::string& field1, const std::string& field2, const std::string& key) const {
        int result_global = lua_getglobal(this->L, field1.c_str());
        if(result_global == LUA_TNIL)
            return 0.0f;
    
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field1 << " is not a table.\n";
            return 0.0f;
        }

        lua_pushstring(this->L, field2.c_str());
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return 0.0f;

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field2 << " is not a table.\n";
            return 0.0f;
        }
        
        lua_pushstring(this->L, key.c_str());
        lua_gettable(this->L, -2);
        float result = (float)lua_tonumber(this->L, -1);
        lua_pop(this->L, 1);
        
        lua_pop(this->L, 2);
        return result;
    }

    bool Driver::getNestedFieldValueBoolean(const std::string& field1, const std::string& field2, const std::string& key) const {
        int result_global = lua_getglobal(this->L, field1.c_str());
        if(result_global == LUA_TNIL)
            return false;
    
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field1 << " is not a table.\n";
            return false;
        }

        lua_pushstring(this->L, field2.c_str());
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return false;

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field2 << " is not a table.\n";
            return false;
        }
        
        lua_pushstring(this->L, key.c_str());
        lua_gettable(this->L, -2);
        bool result = lua_toboolean(this->L, -1);
        lua_pop(this->L, 1);
        
        lua_pop(this->L, 2);
        return result;
    }

    sf::Vector2i Driver::getNestedFieldValueVector2i(const std::string& field1, const std::string& field2, const std::string& key1, const std::string& key2) const {
        int result_global = lua_getglobal(this->L, field1.c_str());        
        if(result_global == LUA_TNIL)
            return sf::Vector2i(0, 0);
        
        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field1 << " is not a table.\n";
            return sf::Vector2i(0, 0);
        }

        lua_pushstring(this->L, field2.c_str());
        int result_table = lua_gettable(this->L, -2);
        if(result_table == LUA_TNIL)
            return sf::Vector2i(0, 0);

        if(!lua_istable(this->L, -1)) {
            std::cout << "[Lua Driver Error]: " << field2 << " is not a table.\n";
            return sf::Vector2i(0, 0);
        }

        sf::Vector2i result;

        lua_pushstring(this->L, key1.c_str());
        lua_gettable(this->L, -2);
        result.x = lua_tointeger(this->L, -1);
        lua_pop(this->L, 1);

        lua_pushstring(this->L, key2.c_str());
        lua_gettable(this->L, -2);
        result.y = lua_tointeger(this->L, -1);
        lua_pop(this->L, 1);

        lua_pop(this->L, 2);
        return result;
    }

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
                    std::cout << lua_tonumber(this->L, i) << "\n";
                    break;
                case LUA_TSTRING:
                    std::cout << lua_tostring(this->L, i) << "\n";
                    break;
                case LUA_TBOOLEAN:
                    std::cout << lua_toboolean(this->L, i) << "\n";
                    break;
                case LUA_TNIL:
                    std::cout << "Nil\n";
                    break;
                case LUA_TTABLE:
                    std::cout << "Table\n";
                    break;
            }
        }

        std::cout << "==========================\n";
    }

    iso::Settings Driver::getApplicationSettings() const {
        return this->app_settings;
    }
}
}