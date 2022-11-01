#pragma once

#include "windowManager.hpp"
#include "resourceManager.hpp"
#include "gamestateManager.hpp"
#include "worldGenerator.hpp"
#include "types.hpp"

#include "player.hpp"

#include "building.hpp"
#include "resource.hpp"
#include "item.hpp"
#include "biome.hpp"
#include "luascript.hpp"
#include "schedule.hpp"

#include <SFML/System.hpp>
#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <vector>
#include <string>

namespace iso {
class SimulationManager {
    private:
        void loadApplicationConfig();
        void loadGameData();
        void emitEvents();

    private:
        lua_State* L;

    public:
        WindowManager    window;
        ResourceManager  resource;   
        GamestateManager gamestate;
        WorldGenerator   world_generator;
        Schedule         schedule; // Interval based events.

        std::vector <Region> world_map;
        std::map    <int, GameObject> forests;
        std::map    <int, GameObject> rivers;
        std::map    <int, GameObject> lakes;

        std::vector <Player> players;

        int time_s;             // Real time since start of the game.
        int simulation_time_s;  // Simulation time since start of the game.
        sf::Time time_since_start;
        sf::Time simulation_time_since_start;
        sf::Clock clock;
        sf::Clock simulation_clock;
        int fps;
        float time_per_frame;
        int font_size;

    public:
        SimulationManager();
        ~SimulationManager();

        std::vector <int> wAstar(int start, int end) const; // Worldmap.
        std::vector <int> rAstar(int start, int end) const; // Regionmap.

        void loop();
        lua_State* lua() const;

        Player* getHumanPlayer();
        Player* getPlayer(int player_id);
        bool isHumanPlayer(int player_id) const;
        bool checkPlayerExists(int player_id) const;
        void addPlayer(Player player);
};

void exitApplication(int code);
void handleScriptableEvent(const std::string& event_name);
}

namespace lua {
void runLuaFile(const std::string& filename);

std::string                readString(luabridge::LuaRef, bool throw_if_null = false);
int                        readInteger(luabridge::LuaRef, bool throw_if_null = false);
float                      readNumber(luabridge::LuaRef, bool throw_if_null = false);
bool                       readBoolean(luabridge::LuaRef, bool throw_if_null = false);
core::Colour               readColour(luabridge::LuaRef, bool throw_if_null = false);
core::Vector2i             readVector2i(luabridge::LuaRef, bool throw_if_null = false);
core::Vector2f             readScreenRatio(luabridge::LuaRef, bool throw_if_null = false);
std::vector <std::string>  readVectorString(luabridge::LuaRef, bool throw_if_null = false);
std::vector <core::Colour> readVectorColour(luabridge::LuaRef, bool throw_if_null = false);
std::vector <iso::BuildingHarvest> getBuildingHarvest(luabridge::LuaRef);
std::vector <iso::BuildingProduction> getBuildingProduction(luabridge::LuaRef);
}

extern iso::SimulationManager game_manager;
extern std::vector <std::string> event_queue;