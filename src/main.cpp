#include "simulationManager.hpp"
#include "generationSettings.hpp"

#include "building.hpp"
#include "resource.hpp"
#include "item.hpp"
#include "biome.hpp"
#include "luascript.hpp" 
#include "luadriver.hpp"

// Define the 'extern' variables here.

std::vector <iso::Building>    buildings;
std::vector <iso::Resource>    resources;
std::vector <iso::StorageItem> items;
std::vector <iso::Biome>       biomes;
std::vector <lua::LuaScript>   scripts;
std::vector <std::string>      event_queue;
iso::GameSettings              game_settings;

int main() {
    static iso::SimulationManager game_manager;
    game_manager.loop();
    iso::exitApplication(0);
}

