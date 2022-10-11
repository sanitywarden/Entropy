#include "simulationManager.hpp"
#include "gamestate.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"
#include "worldData.hpp"
#include "colours.hpp"
#include "globalutilities.hpp"

#include <queue>
#include <iostream>
#include <filesystem>

namespace iso {
SimulationManager::SimulationManager() {
    this->L = luaL_newstate();
    luaL_openlibs(this->L);

    sf::Clock clock;

    // BIND C++ TO LUA
    registerLua();

    // LOAD APPLICATION CONFIG
    loadApplicationConfig();

    // LOAD GAME DATA
    std::cout << "[] Loading game data.\n";
    loadGameData();

    this->font_size = (this->window.getWindowWidth() + this->window.getWindowHeight()) / 160;

    auto time_rounded = std::ceil(clock.getElapsedTime().asSeconds() * 100) / 100;
    std::cout << "[] Initialised in " << time_rounded << "s.\n";

    static auto worldmap = std::shared_ptr <Gamestate> (new Worldmap());
    this->gamestate.addGamestate("Worldmap", worldmap);
    this->world_generator.generate();

    static auto regionmap = std::shared_ptr <Gamestate> (new Regionmap());
    this->gamestate.addGamestate("Regionmap", regionmap);

    this->gamestate.setGamestate("Worldmap");
}

SimulationManager::~SimulationManager() {
    lua_close(this->L);
}

void SimulationManager::loop() {
    this->clock = sf::Clock();
    this->simulation_clock = sf::Clock();

    auto second = sf::seconds(1.0f);
    auto simulation_speed = sf::seconds(world_data.simulation_speed);

    // Frames per second.
    int updates = 0;

    while(this->window.isOpen()) {
        if(this->time_since_start.asMilliseconds() < second.asMilliseconds()) {
            // Measure the difference in time between the last frame and this frame.
            float delta_time = this->clock.getElapsedTime().asMilliseconds() - this->time_since_start.asMilliseconds();            
            this->time_since_start = this->clock.getElapsedTime();
            updates++;

            /* Delta time is saved as milliseconds, convert that to seconds.
             * 1ms -> 0.001s. */
            float delta_time_s = delta_time / 1000;
            
            auto* gamestate = this->gamestate.getGamestate();
            if(!gamestate) {
                printError("SimulationManager::loop()", "Current gamestate is a nullptr.\n");
                return;
            }

            this->emitEvents();
            
            gamestate->update(delta_time_s);
            gamestate->render(delta_time_s);
        
            event_queue.resize(0);
        }

        else {
            this->fps = updates;
            this->time_per_frame = (float)this->time_since_start.asMilliseconds() / (float)this->fps;
            this->clock.restart();
            this->time_since_start = sf::Time::Zero;

            this->time_s++;

            for(auto& event : this->schedule) {
                if(event.getCurrentTime() != event.getRequiredTime() && !event.isSpeedDependant())
                    event.progressEvent();
            }

            updates = 0;
        }

        if(this->simulation_time_since_start.asMilliseconds() < simulation_speed.asMilliseconds())
            this->simulation_time_since_start = this->simulation_clock.getElapsedTime();

        else {
            this->simulation_clock.restart();
            this->simulation_time_since_start = sf::Time::Zero;

            this->simulation_time_s++;

            // Update gamestate-specific scheduler.
            auto* gamestate = this->gamestate.getGamestate();
            if(gamestate)
                gamestate->updateScheduler();

            for(auto& event : this->schedule) {
                if(event.getCurrentTime() != event.getRequiredTime() && event.isSpeedDependant())
                    event.progressEvent();
            }
        }
    }
}

lua_State* SimulationManager::lua() const {
    return this->L;
}

struct aNode {
    int x;
    int y;
    int index;
    int h_cost;
    int g_cost;
    bool passable;
};

std::vector <int> SimulationManager::wAstar(int start, int end) const {
    const auto H = [](const aNode& node, const aNode& end) -> int {      
        return std::abs(end.x - node.x) + std::abs(end.y - node.y);
    };

    const auto neighbours = [this](core::Vector2i grid_position) -> std::vector <int> {
        auto grid_up    = grid_position + core::Vector2i(0, 1);
        auto grid_down  = grid_position + core::Vector2i(0, -1);
        auto grid_left  = grid_position + core::Vector2i(-1, 0);
        auto grid_right = grid_position + core::Vector2i(1, 0);

        auto index = calculateWorldIndex(grid_position);

        std::vector <int> neighbours;
        if(inWorldBounds(grid_left))
            neighbours.push_back(index - 1);

        if(inWorldBounds(grid_right))
            neighbours.push_back(index + 1);

        if(inWorldBounds(grid_up))
            neighbours.push_back(index - world_data.w_width);

        if(inWorldBounds(grid_down))
            neighbours.push_back(index + world_data.w_width);

        return neighbours;
    };

    const auto passable = [this](int index) -> bool {
        const auto& region = this->world_map[index];
        if(region.regiontype.is_ocean())
            return false;
        return true;
    };

    std::vector <aNode> nodes(getWorldSize());

    for(int y = 0; y < world_data.w_width; y++) {
        for(int x = 0; x < world_data.w_width; x++) {
            const int index = calculateWorldIndex(x, y);
            aNode& node = nodes[index];

            node.index = index;        
            node.x = x;
            node.y = y;
            node.passable = passable(index); 
        }
    }

    const auto& node_start = nodes[start];
    const auto& node_end   = nodes[end];

    if(!node_end.passable)
        return std::vector<int> ();

    typedef std::pair<int, int> Intpair;
    typedef std::priority_queue <Intpair, std::vector <Intpair>, std::greater <Intpair>> Frontier;

    Frontier frontier;
    std::vector <int> seen;
    std::unordered_map <int, int> cost_so_far; // index, cost
    std::unordered_map <int, int> came_from;   // index, index

    const auto reconstruct = [](int start, int end, std::unordered_map <int, int> came_from) -> std::vector <int> {
        std::vector <int> path;
        int current = end;
        while(current != start) {
            path.push_back(current);
            current = came_from[current];
        }

        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return path;
    };

    frontier.push(Intpair(H(node_start, node_end), node_start.index));
    cost_so_far[node_start.index] = 0;

    while(!frontier.empty()) {
        const aNode& current_node = nodes[(frontier.top()).second];
        frontier.pop();

        if(current_node.index == end)
            break;

        for(int index : neighbours(core::Vector2i(current_node.x, current_node.y))) {
            const aNode& neighbour = nodes[index];
            const int new_cost   = cost_so_far[current_node.index] + 1;

            if(!neighbour.passable)
                continue;

            if(cost_so_far.find(index) == cost_so_far.end() || new_cost < cost_so_far[index]) {
                cost_so_far[index] = new_cost;
                came_from[index]   = current_node.index;
                
                const int priority = new_cost + H(neighbour, node_end);
                frontier.push(Intpair(priority, index));
            }
        }
    }

    return reconstruct(start, end, came_from);
}

std::vector <int> SimulationManager::rAstar(int start, int end) const {
    auto* regionmap = (Regionmap*)this->gamestate.getGamestateByName("Regionmap");
    const auto& region = this->world_map[regionmap->getRegionIndex()];

    const auto H = [](const aNode& node, const aNode& end) -> int {
        // Euclidean distance.
        // return std::sqrt(
        //     std::pow(node.x - end.x, 2) + std::pow(node.y - end.y, 2)
        // );

        // Manhattan distance.  
        return std::abs(end.x - node.x) + std::abs(end.y - node.y);
    };

    const auto G = [H](const aNode& start_node, const aNode& current_node) -> int {
        return H(start_node, current_node);
    };

    const auto neighbours = [region, this](core::Vector2i grid_position) -> std::vector <int> {
        auto grid_up    = grid_position + core::Vector2i(0, 1);
        auto grid_down  = grid_position + core::Vector2i(0, -1);
        auto grid_left  = grid_position + core::Vector2i(-1, 0);
        auto grid_right = grid_position + core::Vector2i(1, 0);

        auto index = calculateRegionIndex(grid_position);
        std::vector <int> neighbours;
        
        if(inRegionBounds(grid_left))
            if(region.isPassableRegionmap(calculateRegionIndex(grid_left)))
                neighbours.push_back(index - 1);

        if(inRegionBounds(grid_right))
            if(region.isPassableRegionmap(calculateRegionIndex(grid_right)))
                neighbours.push_back(index + 1);

        if(inRegionBounds(grid_up))
            if(region.isPassableRegionmap(calculateRegionIndex(grid_up)))
                neighbours.push_back(index - world_data.r_width);

        if(inRegionBounds(grid_down))
            if(region.isPassableRegionmap(calculateRegionIndex(grid_down)))
                neighbours.push_back(index + world_data.r_width);

        return neighbours;
    };
    
    std::vector <aNode> nodes(getRegionSize());
    auto& node_start = nodes[start];
    auto& node_end   = nodes[end];

    node_start.index = start;
    node_start.passable = region.isPassableRegionmap(start);

    node_end.index = end;
    node_end.passable = region.isPassableRegionmap(end);

    if(!node_end.passable)
        return std::vector<int> ();

    for(int y = 0; y < world_data.r_width; y++) {
        for(int x = 0; x < world_data.r_width; x++) {
            const int index = calculateRegionIndex(x, y);
            aNode& node = nodes[index];

            node.index = index;        
            node.x = x;
            node.y = y;
            node.passable = region.isPassableRegionmap(index); 
            node.h_cost = H(node, node_end);
            node.g_cost = G(node_start, node);
        }
    }

    typedef std::pair<int, int> Intpair;
    typedef std::priority_queue <Intpair, std::vector <Intpair>, std::greater <Intpair>> Frontier;

    Frontier frontier;
    std::vector <int> seen;
    std::unordered_map <int, int> cost_so_far; // index, cost
    std::unordered_map <int, int> came_from;   // index, index

    frontier.push(Intpair(H(node_start, node_end), node_start.index));
    cost_so_far[node_start.index] = 0;

    const auto get_node_movement_cost = [region](const aNode& current, const aNode& next) -> int {
        const auto& tile = region.map[current.index];
        if(tile.isRiver())
            return 3;
    
        if(region.treeExistsAt(current.index))
            return 5;

        return 1;
    };

    const auto reconstruct_path = [](const aNode& start, const aNode& end, std::unordered_map <int, int> came_from) -> std::vector <int> {
        std::vector <int> path;
        int current_index = end.index;
        while(current_index != start.index) {
            path.push_back(current_index);
            current_index = came_from[current_index];
        }

        path.push_back(start.index);
        std::reverse(path.begin(), path.end());
        return path;
    };

    while(!frontier.empty()) {
        const auto& current_node = nodes[(frontier.top()).second];
        frontier.pop();
        
        if(current_node.index == end)
            return reconstruct_path(node_start, node_end, came_from);

        seen.push_back(current_node.index);

        auto current_neighbours = neighbours(core::Vector2i(current_node.x, current_node.y));
        for(auto index : current_neighbours) {
            // If index was already seen, skip.
            if(std::find(seen.begin(), seen.end(), index) != seen.end())
                continue;

            const auto& neighbour = nodes[index];
            if(!neighbour.passable)
                continue;

            auto new_cost = cost_so_far[current_node.index] + get_node_movement_cost(current_node, neighbour);
            if(cost_so_far.find(index) == cost_so_far.end() || new_cost < cost_so_far[index]) {
                cost_so_far[index] = new_cost;
                auto priority = new_cost + H(neighbour, node_end);
                frontier.push({ priority, index });
                came_from[index] = current_node.index;
            }
        }
    }

    return reconstruct_path(node_start, node_end, came_from);
}

Player* SimulationManager::getHumanPlayer() {
    if(!this->checkPlayerExists(0)) {
        printError("PlayerManager::getHumanPlayer()", "There is no human player.");
        return nullptr;
    }

    return &this->players.at(0);
}

Player* SimulationManager::getPlayer(int player_id) {
    if(!this->checkPlayerExists(player_id)) {
        printError("PlayerManager::getPlayer()", "Player with id '" + std::to_string(player_id) + " does not exist.");
        return nullptr;
    }

    return &this->players.at(player_id);
}

bool SimulationManager::isHumanPlayer(int player_id) const {
    return player_id == 0;
}

bool SimulationManager::checkPlayerExists(int player_id) const {
    for(auto& player : this->players)
        if(player.getID() == player_id)
            return true;
    return false;
}

void SimulationManager::addPlayer(Player player) {
    this->players.push_back(player);
}

void L_loadTexture(const std::string& filename, const std::string& id, core::Vector2i position, core::Vector2i size) {
    if(!game_manager.resource.checkTextureExists(id))
        game_manager.resource.loadTexture(filename, id, sf::IntRect(position.asSFMLVector2i(), size.asSFMLVector2i()));
}

void L_loadFont(const std::string& filename, const std::string& id) {
    if(!game_manager.resource.checkFontExists(id))
        game_manager.resource.loadFont(filename, id);
}

void L_loadEvent(const std::string& id, float required_time, bool speed_dependant) {  
    EventData data;
    data.name = id;
    data.time = 0;
    data.required_time = required_time;
    data.speed_dependant = speed_dependant;

    ScheduledEvent event(data);
    game_manager.schedule.push_back(event);
}

void L_createIcon(const std::string& id) {
    if(!game_manager.resource.checkTextureExists(id)) {
        printError("L_createIcon()", "Tried to create icon from not existing texure '" + id + "'");
        return;
    }

    const auto& texture = game_manager.resource.getTexture(id);
    auto save_as = "icon_" + readAfter(id, "icon_template_");
    auto base    = "icon_default";
    game_manager.resource.blendTextures(save_as, base, id); 
}

void L_setGamestate(const std::string& id) {
    game_manager.gamestate.setGamestate(id);
}

void L_showInterface(const std::string& id) {
    auto gamestate = game_manager.gamestate.getGamestate();
    gamestate->setVisibilityTrue(id);
}

void L_hideInterface(const std::string& id) {
    auto gamestate = game_manager.gamestate.getGamestate();
    gamestate->setVisibilityFalse(id);
}

bool L_isInterfaceVisible(const std::string& id) {
    auto gamestate = game_manager.gamestate.getGamestate();
    return gamestate->isComponentVisible(id);
}

bool L_isKeyPressed(const std::string& key_id) {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    return current_gamestate->controls.isKeyPressed(key_id);
}

void exitApplication(int code) {
    if(code)
        printError("exitApplication()", "Quit with code " + std::to_string(code));
    exit(code);
}

void SimulationManager::registerLua() {
    std::cout << "[] Binding C++ to Lua.\n";

    luabridge::getGlobalNamespace(game_manager.lua())
        .beginClass <core::Colour> ("Colour")
            .addConstructor <void (*) (uint8_t, uint8_t, uint8_t, uint8_t)> ()
            .addProperty("r", core::Colour::getR, core::Colour::setR)
            .addProperty("g", core::Colour::getG, core::Colour::setG)
            .addProperty("b", core::Colour::getB, core::Colour::setB)
            .addProperty("a", core::Colour::getA, core::Colour::setR)
        .endClass()
        .beginClass <core::Vector2f> ("Vector2f")
            .addConstructor <void (*) (float, float)> ()
            .addProperty("x", core::Vector2f::getX, core::Vector2f::setX) 
            .addProperty("y", core::Vector2f::getY, core::Vector2f::setY) 
        .endClass()
        .beginClass <core::Vector2i> ("Vector2i")
            .addConstructor <void (*) (float, float)> ()
            .addProperty("x", core::Vector2i::getX, core::Vector2i::setX) 
            .addProperty("y", core::Vector2i::getY, core::Vector2i::setY) 
        .endClass()
        .beginClass <core::Vector3f> ("Vector3f")
            .addConstructor <void (*) (float, float, float)> ()
            .addProperty("x", core::Vector3f::getX, core::Vector3f::setX) 
            .addProperty("y", core::Vector3f::getY, core::Vector3f::setY) 
            .addProperty("y", core::Vector3f::getZ, core::Vector3f::setZ) 
        .endClass()
        .beginClass <core::Vector3i> ("Vector3i")
            .addConstructor <void (*) (float, float, float)> ()
            .addProperty("x", core::Vector3i::getX, core::Vector3i::setX) 
            .addProperty("y", core::Vector3i::getY, core::Vector3i::setY) 
            .addProperty("y", core::Vector3i::getZ, core::Vector3i::setZ) 
        .endClass()
        .beginClass <core::Ratio> ("Ratio")
            .addConstructor <void (*) (std::string, std::string)> ()
        .endClass()
        .beginClass <Building> ("Building")
            .addConstructor <void (*) ()> ()
            .addProperty("name"       , Building::getBuildingName       , Building::setBuildingName)
            .addProperty("description", Building::getBuildingDescription, Building::setBuildingDescription)
            .addProperty("area"       , Building::getBuildingArea       , Building::setBuildingArea)
            .addProperty("texture"    , Building::getBuildingTexture    , Building::setBuildingTexture)
            .addProperty("scan_area"  , Building::getBuildingScanArea   , Building::setBuildingScanArea)
            .addProperty("bcost"      , Building::getBuildingCost       , Building::setBuildingCost)
            .addProperty("icon"       , Building::getBuildingIcon       , Building::setBuildingIcon)
            .addProperty("icon_size"  , Building::getBuildingIconSize   , Building::setBuildingIconSize)
            .addProperty("harvest"    , Building::getBuildingHarvests   , Building::setBuildingHarvests)
            .addProperty("production" , Building::getBuildingProduction , Building::setBuildingProduction)
            .addProperty("removable"  , Building::isRemovable           , Building::setRemovable)
        .endClass()
        .beginClass <Biome> ("Biome")
            .addConstructor <void (*) ()> ()
            .addProperty("name"               , Biome::getBiomeName            , Biome::setBiomeName)
            .addProperty("id"                 , Biome::getBiomeId              , Biome::setBiomeId)
            .addProperty("description"        , Biome::getBiomeDescription     , Biome::setBiomeDescription)
            .addProperty("wmap_colour"        , Biome::getBiomeWorldmapColour  , Biome::setBiomeWorldmapColour)
            .addProperty("temperature"        , Biome::getBiomeTemperature     , Biome::setBiomeTemperature)
            .addProperty("moisture"           , Biome::getBiomeMoisture        , Biome::setBiomeMoisture)
            .addProperty("wmap_forest_texture", Biome::getWorldmapForestTexture, Biome::setWorldmapForestTexture)
            .addProperty("tile_list"          , Biome::getBiomeTileList        , Biome::setBiomeTileList)
            .addProperty("tree_list"          , Biome::getBiomeTreeList        , Biome::setBiomeTreeList)
        .endClass()
        .beginClass <Region> ("Region")
            .addFunction("getTileAtIndex", &Region::getTileAtIndex)
            .addFunction("isBiome"       , &Region::isBiome)
        .endClass()
        .beginClass <Tile> ("Tile")
            .addFunction("isTerrain", &Tile::isTerrain)
            .addFunction("isWater"  , &Tile::isWater)
            .addFunction("isOcean"  , &Tile::isOcean)
            .addFunction("isRiver"  , &Tile::isRiver)
            .addFunction("isCoast"  , &Tile::isCoast)
        .endClass()
        .addFunction("loadTexture"           , &L_loadTexture)
        .addFunction("loadFont"              , &L_loadFont)
        .addFunction("loadScheduledEvent"    , &L_loadEvent)
        .addFunction("createIcon"            , &L_createIcon)
        .addFunction("setGamestate"          , &L_setGamestate)
        .addFunction("isKeyPressed"          , &L_isKeyPressed)
        .addFunction("showInterface"         , &L_showInterface)
        .addFunction("hideInterface"         , &L_hideInterface)
        .addFunction("isInterfaceVisible"    , &L_isInterfaceVisible)
    ;
}

void SimulationManager::loadApplicationConfig() {
    std::cout << "[] Reading application config.\n";
    {
        luaL_dofile(this->L, "./data/appconfig.lua");
        auto appconfig = luabridge::getGlobal(this->L, "Appconfig");

        ApplicationSettings settings;
        settings.window_size            = appconfig["window_size"];
        settings.window_fullscreen      = appconfig["fullscreen"];
        settings.window_vsync           = appconfig["vsync"];
        settings.window_refresh_rate    = appconfig["refresh_rate"];

        this->window.createWindow(settings, "Entropy by Vivit");
        this->window.setKeyHold(false);
    }

    std::cout << "[] Reading world generation data.\n";
    {
        std::string generationdata_path = "./data/world_generation.lua";
        luaL_dofile(this->L, generationdata_path.c_str());

        auto worldconfig = luabridge::getGlobal(this->L, "GameSettings");

        iso::WorldData settings;
        settings.r_persistence            = worldconfig["region_persistence"];
        settings.w_persistence            = worldconfig["world_persistence"];
        settings.simulation_speed         = worldconfig["simulation_update_frequency"];
        settings.w_width                  = worldconfig["World"]["size"];
        settings.w_terrain_from           = worldconfig["World"]["terrain_from"];
        settings.w_forest_from            = worldconfig["World"]["forest_from"];
        settings.fog_of_war_enabled       = worldconfig["World"]["fog_of_war_enabled"];
        settings.w_temperature_multiplier = worldconfig["World"]["multiplier_temperature"];
        settings.w_moisture_multiplier    = worldconfig["World"]["multiplier_moisture"];
        settings.r_width                  = worldconfig["Region"]["size"];
        settings.building_cost_enabled    = worldconfig["Region"]["building_cost_enabled"];

        world_data = settings;
    }
}

void SimulationManager::loadGameData() {
    lua::runLuaFile("./src/scripts/resource/load_ui.lua");
    lua::runLuaFile("./src/scripts/resource/load_scheduled_events.lua");
    lua::runLuaFile("./src/scripts/resource/load_worldmap.lua");
    lua::runLuaFile("./src/scripts/resource/load_regionmap.lua");

    namespace fs = std::filesystem;

    // BIOMES 
    {
        std::string biomedata_path = "./data/biome/";
        std::cout << "[] Loading biomes.\n";
        for(const auto& file : fs::directory_iterator(biomedata_path)) {
            const auto& file_path = file.path().string();                      
            const auto& file_path_extension = file.path().extension().string();

            if(file_path_extension == ".lua") {
                std::cout << "  [] Registering biome '" << file_path << "'.\n";
                luaL_dofile(this->L, file_path.c_str()); 

                auto biomedata = luabridge::getGlobal(this->L, "Biome");

                iso::BiomeData data;
                data.filename                = file_path;
                data.name                    = lua::readString(biomedata["name"]);
                data.id                      = lua::readString(biomedata["id"]);
                data.description             = lua::readString(biomedata["description"]);
                data.colour_wmap             = lua::readColour(biomedata["colour_wmap"]);
                data.temperature             = lua::readString(biomedata["temperature"]);
                data.moisture                = lua::readString(biomedata["moisture"]);
                data.can_be_forest           = lua::readBoolean(biomedata["can_be_forest"]);
                data.forest_texture_worldmap = lua::readString(biomedata["forest_texture_worldmap"]);
                data.tiles                   = lua::readVectorString(biomedata["tiles"]);
                data.trees                   = lua::readVectorString(biomedata["trees"]);

                iso::Biome biome(data);
                BIOME_TABLE.push_back(biome);
            }
        }
    }

    // ITEM DATA
    {
        std::string resourcedata_path = "./data/item/";
        std::cout << "[] Loading items.\n";
        for(const auto& file : fs::directory_iterator(resourcedata_path)) {
            const auto& file_path = file.path().string();
            const auto& file_path_extension = file.path().extension().string();

            if(file_path_extension == ".lua") {
                std::cout << "  []: Registering item '" << file_path << "'.\n";
                luaL_dofile(this->L, file_path.c_str());

                auto itemdata = luabridge::getGlobal(this->L, "Item");

                iso::ItemData data;
                data.filename     = file_path;
                data.name         = lua::readString(itemdata["name"]);
                data.description  = lua::readString(itemdata["description"]);
                data.icon_texture = lua::readString(itemdata["icon_texture"]);
                data.icon_size    = lua::readVector2i(itemdata["icon_size"]);
                data.type         = lua::readString(itemdata["type"]);
                data.amount       = 0;
            
                iso::StorageItem item(data);
                ITEM_TABLE.push_back(item);
            }
        }   
    }

    // RESOURCES
    {
        std::string resourcedata_path = "./data/resource/";
        std::cout << "[] Loading resources.\n";
        for(const auto& file : fs::directory_iterator(resourcedata_path)) {
            const auto& file_path = file.path().string();
            const auto& file_path_extension = file.path().extension().string();

            if(file_path_extension == ".lua") {
                std::cout << "  []: Registering resource '" << file_path << "'.\n";
                luaL_dofile(this->L, file_path.c_str());

                auto resourcedata = luabridge::getGlobal(this->L, "Resource");

                iso::ResourceData data;
                data.filename            = file_path;
                data.name                = lua::readString(resourcedata["name"]);
                data.description         = lua::readString(resourcedata["description"]);
                data.texture             = lua::readString(resourcedata["texture"]);
                data.texture_size        = lua::readVector2i(resourcedata["texture_size"]);
                data.icon_texture        = lua::readString(resourcedata["icon_texture"]);
                data.icon_size           = lua::readVector2i(resourcedata["icon_size"]);
                data.type                = lua::readString(resourcedata["type"]);
                data.min_occurence       = lua::readInteger(resourcedata["minimum_occurence"]);
                data.max_occurence       = lua::readInteger(resourcedata["maximum_occurence"]);
                data.chance              = lua::readNumber(resourcedata["generation_chance"]);
                data.patch_size          = lua::readInteger(resourcedata["patch_size"]);
                data.tile_requirements   = lua::readVectorString(resourcedata["tile"]);
                data.region_requirements = lua::readVectorString(resourcedata["region"]); 

                iso::Resource resource(data);
                RESOURCE_TABLE.push_back(resource);   
            }
        }
    }

    // BUILDINGS
    {
        std::string buildingdata_path = "./data/building/";
        std::cout << "[] Loading buildings.\n";
        for(const auto& file : fs::directory_iterator(buildingdata_path)) {
            const auto& file_path = file.path().string();                      
            const auto& file_path_extension = file.path().extension().string();

            if(file_path_extension == ".lua") {
                std::cout << "  [] Registering building '" << file_path << "'.\n";
                luaL_dofile(this->L, file_path.c_str());

                auto buildingdata = luabridge::getGlobal(this->L, "Building");

                iso::BuildingData data;
                data.filename     = file_path;                
                data.name         = lua::readString(buildingdata["name"]);
                data.description  = lua::readString(buildingdata["description"]);
                data.size         = lua::readVector2i(buildingdata["size"]);
                data.texture      = lua::readString(buildingdata["texture"]);
                data.texture_size = lua::readVector2i(buildingdata["texture_size"]);
                data.icon_texture = lua::readString(buildingdata["icon_texture"]);
                data.icon_size    = lua::readVector2i(buildingdata["icon_size"]);
                data.removable    = lua::readBoolean(buildingdata["removable"]);
                data.scan_area    = lua::readVector2i(buildingdata["scan_area"]);
                data.harvests     = lua::getBuildingHarvest(buildingdata["harvests"]);
                data.produces     = lua::getBuildingProduction(buildingdata["produces"]);

                iso::Building building(data);
                BUILDING_TABLE.push_back(building);
            }
        }
    }

    // SCRIPTS
    {
        std::string scripts_path = "./src/scripts/";
        std::cout << "[] Loading scripts.\n";
        for(const auto& file : fs::directory_iterator(scripts_path)) {
            const auto& file_path = file.path().string();

            if(!containsWord(file_path, "game_"))
                continue;

            for(const auto& nested_file : fs::directory_iterator(file_path)) {
                const auto& nested_file_extension = nested_file.path().extension();

                if(nested_file_extension == ".lua") {
                    lua::LuaScript script(nested_file.path().string());
                    SCRIPT_TABLE.push_back(script);
                }
            }
        }
    }

    std::cout << "[] Loaded " << BIOME_TABLE.size()    << " biomes.\n";
    std::cout << "[] Loaded " << ITEM_TABLE.size()     << " items.\n";
    std::cout << "[] Loaded " << RESOURCE_TABLE.size() << " resources.\n";
    std::cout << "[] Loaded " << BUILDING_TABLE.size() << " buildings.\n";
    std::cout << "[] Loaded " << SCRIPT_TABLE.size()   << " scripts.\n"; 
}   

void SimulationManager::emitEvents() {
    auto* gamestate = this->gamestate.getGamestate();

    if(!gamestate->controls.key_map.size()) {
        for(const auto& pair : gamestate_keys)
            gamestate->controls.addKeyMapping(pair.first, pair.second);
    }

    for(auto& event : this->schedule) {
        if(event.getCurrentTime() == event.getRequiredTime()) {
            for(auto& script : SCRIPT_TABLE)
                script.onEvent(event.getEventName());

            event.resetTime();
        }
    }

    gamestate->controls.mouse_dragged = gamestate->controls.mouse_moved && gamestate->controls.mouse_middle;
    while(this->window.getWindow()->pollEvent(gamestate->event)) {
        switch(gamestate->event.type) {
            case sf::Event::Closed: {
                exitApplication(0);
                break;
            };

            case sf::Event::Resized: {
                gamestate->controls.resized = sf::Vector2f(gamestate->event.size.width, gamestate->event.size.height);
                
                auto new_window_size = this->window.getWindowSize();
                this->font_size = (this->window.getWindowWidth() + this->window.getWindowHeight()) / 160;
                event_queue.push_back("WINDOW_RESIZE");
                
                break;
            }

            case sf::Event::KeyPressed: {
                for(const auto& pair : gamestate->controls.key_map) {
                    const auto& name = pair.first;
                    auto state = sf::Keyboard::isKeyPressed(gamestate->controls.key_map[name]);

                    if(name == "SHIFT")
                        state = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift); 

                    else if(name == "CTRL")
                        state = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl); 

                    else if(name == "ALT")
                        state = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt); 

                    else if(name == "SYSTEM")
                        state = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem); 

                    if(gamestate->controls.key_state.count(name)) {
                        gamestate->controls.key_state[name] = state;
                    }
                    else gamestate->controls.key_state.insert({ name, state });

                    if(state)
                        gamestate->controls.last_key_name = name;
                }                

                game_manager.window.getWindow()->setView(gamestate->view_interface);
                gamestate->updateUI();
                game_manager.window.getWindow()->setView(gamestate->view_game);

                // Blocks keyboard input for non-ui input.
                gamestate->controls.blockKeyboardInput(gamestate->mouseIntersectsUI());

                gamestate->runGUIEventHandle("onKeyPress");
                event_queue.push_back("BUTTON_PRESSED");

                gamestate->controls.last_key_name = "";
                break;
            }

            case sf::Event::KeyReleased: {
                for(const auto& pair : gamestate->controls.key_map) {
                    const auto& name  = pair.first;
                    const auto  state = sf::Keyboard::isKeyPressed(gamestate->controls.key_map[name]);

                    if(gamestate->controls.key_state.count(name)) {
                        gamestate->controls.key_state[name] = state;
                    }
                    else gamestate->controls.key_state.insert({ name, state });
                }
                
                game_manager.window.getWindow()->setView(gamestate->view_interface);
                gamestate->updateUI();
                game_manager.window.getWindow()->setView(gamestate->view_game);

                gamestate->runGUIEventHandle("onKeyRelease");
                event_queue.push_back("BUTTON_RELEASED");
                
                break;
            }

            case sf::Event::MouseButtonPressed: {
                gamestate->controls.mouse_left   = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left); 
                gamestate->controls.mouse_right  = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right); 
                gamestate->controls.mouse_middle = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);

                if(gamestate->controls.mouse_middle)
                    gamestate->controls.button_position_pressed = gamestate->mouse_position_window;

                gamestate->runGUIEventHandle("onMouseButtonPress");

                if(gamestate->controls.mouse_left) {
                    gamestate->runGUIEventHandle("onLeftMouseButtonPress");
                    event_queue.push_back("LMB_PRESSED");
                }
                
                if(gamestate->controls.mouse_right) {
                    gamestate->runGUIEventHandle("onRightMouseButtonPress");
                    event_queue.push_back("RMB_PRESSED");
                }

                if(gamestate->controls.mouse_middle) {
                    gamestate->runGUIEventHandle("onMiddleMouseButtonPress");
                    event_queue.push_back("MMB_PRESSED");   
                }

                break;
            }

            case sf::Event::MouseButtonReleased: {
                auto was_middle_pressed = gamestate->controls.mouse_middle;

                gamestate->controls.mouse_left   = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left); 
                gamestate->controls.mouse_right  = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right); 
                gamestate->controls.mouse_middle = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);

                gamestate->controls.mouse_dragged = false;

                if(was_middle_pressed && !gamestate->controls.mouse_middle)
                    gamestate->controls.button_position_released = gamestate->mouse_position_window;

                gamestate->runGUIEventHandle("onMouseButtonRelease");
                event_queue.push_back("MOUSE_BUTTON_RELEASED");

                break;
            }

            case sf::Event::MouseMoved: {
                gamestate->controls.mouse_moved = true;
                
                if(gamestate->controls.mouse_dragged)
                    gamestate->controls.button_position_released = gamestate->mouse_position_window;

                event_queue.push_back("MOUSE_MOVED");
                break;
            }

            case sf::Event::MouseWheelScrolled: {
                gamestate->controls.mouse_middle_down = gamestate->event.mouseWheelScroll.delta == -1;
                gamestate->controls.mouse_middle_up   = gamestate->event.mouseWheelScroll.delta == 1;
                
                gamestate->runGUIEventHandle("onScroll");
                event_queue.push_back("MOUSE_WHEEL_SCROLLED");

                if(gamestate->controls.mouseMiddleUp())
                    gamestate->runGUIEventHandle("onScrollUp");

                if(gamestate->controls.mouseMiddleDown())
                    gamestate->runGUIEventHandle("onScrollDown");

                break;
            }
        }
    }
}
}

namespace lua {
void runLuaFile(const std::string& filename) {
    if(luaL_dofile(game_manager.lua(), filename.c_str())) {
        std::string message = lua_tostring(game_manager.lua(), -1);
        iso::printError("Lua", "A problem occured while executing file '" + filename + "': " + message);
        return;
    }
}

std::string readString(luabridge::LuaRef reference, bool throw_if_null) {
    if(reference.isNil()) {
        if(throw_if_null)
            iso::printError("readString()", "Reference is empty");
        return std::string();
    }
    return (std::string)reference;
}

int readInteger(luabridge::LuaRef reference, bool throw_if_null) {
    if(reference.isNil()) {
        if(throw_if_null)
            iso::printError("readInteger()", "Reference is empty");
        return 0;
    }
    return (int)reference;
}

float readNumber(luabridge::LuaRef reference, bool throw_if_null) {
    if(reference.isNil()) {
        if(throw_if_null)
            iso::printError("readNumber()", "Reference is empty");
        return 0.0f;
    }
        
    return (float)reference;
}

bool readBoolean(luabridge::LuaRef reference, bool throw_if_null) {
    if(reference.isNil()) {
        if(throw_if_null)
            iso::printError("readBoolean()", "Reference is empty");
        return false;
    }
    return (bool)reference;
}

core::Colour readColour(luabridge::LuaRef reference, bool throw_if_null) {
    if(reference.isNil()) {
        if(throw_if_null)
            iso::printError("readColour()", "Reference is empty");
        return core::Colour();
    }
    return (core::Colour)reference;
}

core::Vector2i readVector2i(luabridge::LuaRef reference, bool throw_if_null) {
    if(reference.isNil()) {
        if(throw_if_null)
            iso::printError("readVector2i()", "Reference is empty");
        return core::Vector2i(0, 0);
    }
    return (core::Vector2i)reference;
}

core::Vector2f readScreenRatio(luabridge::LuaRef reference, bool throw_if_null) {
    if(reference.isNil()) {
        if(throw_if_null)
            iso::printError("readScreenRatio()", "Reference is empty");
        return core::Vector2f(0, 0);
    }

    auto ratio = (core::Ratio)reference;
    auto ratio_number = ratio.asNumber();
    return ratio_number;
}

std::vector <std::string> readVectorString(luabridge::LuaRef reference, bool throw_if_null) {
    std::vector <std::string> list;
    if(reference.isNil()) {
        if(throw_if_null)
            iso::printError("readVectorString()", "Reference is empty");
        return list;
    }

    int length = reference.length();
    for(int i = 1; i <= length; i++) {
        list.push_back(reference[i]);
    }

    return list;
}

std::vector <core::Colour> readVectorColour(luabridge::LuaRef reference, bool throw_if_null) {
    std::vector <core::Colour> list;
    if(reference.isNil()) {
        if(throw_if_null)
            iso::printError("readVectorColour()", "Reference is empty");
        return list;
    }
    
    int length = reference.length();
    for(int i = 1; i <= length; i++) {
        list.push_back(reference[i]);
    }

    return list;
}

std::vector <iso::BuildingHarvest> getBuildingHarvest(luabridge::LuaRef reference) {
    std::vector <iso::BuildingHarvest> list;

    if(reference.isNil())
        return list;
    
    int length = reference.length();    
    for(int i = 1; i <= length; i++) {
        iso::BuildingHarvest harvest;

        harvest.name = reference[i]["name"].tostring();
        auto harvest_table = reference[i]["harvest"];
        
        for(int j = 1; j <= harvest_table.length(); j++) {
            for(const auto& resource : RESOURCE_TABLE) {
                auto quantity = harvest_table[resource.getResourceName()];

                if(!quantity.isNil()) {
                    iso::StorageItem item;
                    item = resource;
                    item.setAmount((int)quantity);
                    harvest.harvest.push_back(item);
                }
            }
        }

        list.push_back(harvest);
    }

    return list;
}

std::vector <iso::BuildingProduction> getBuildingProduction(luabridge::LuaRef reference) {
    std::vector <iso::BuildingProduction> list;

    if(reference.isNil())
        return list;
    
    int length = reference.length();
    for(int i = 1; i <= length; i++) {
        iso::BuildingProduction production;

        production.name = reference[i]["name"].tostring();
        production.amount = reference[i]["amount"];

        auto requires_table = reference[i]["requires"];
        for(int j = 1; j <= requires_table.length(); j++) {
            for(const auto& resource : RESOURCE_TABLE) {
                auto quantity = reference[i][resource.getResourceName()];

                if(!quantity.isNil()) {
                    iso::StorageItem item;
                    item = resource;
                    item.setAmount((int)quantity);
                    production.requirements.push_back(item);
                }
            }
        }

        list.push_back(production);
    }

    return list;
}
}