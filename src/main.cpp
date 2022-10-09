#include "building.hpp"
#include "resource.hpp"
#include "item.hpp"
#include "biome.hpp"
#include "worldData.hpp"
#include "luascript.hpp"
#include "simulationManager.hpp"

// Define the 'extern' variables here.

std::vector <iso::Building>    BUILDING_TABLE;
std::vector <iso::Resource>    RESOURCE_TABLE;
std::vector <iso::StorageItem> ITEM_TABLE;
std::vector <iso::Biome>       BIOME_TABLE;
std::vector <lua::LuaScript>   SCRIPT_TABLE;
iso::WorldData                 world_data;
std::vector <std::string>      event_queue;
iso::SimulationManager         game_manager;

#include "interfacePage.hpp"

int main() {
    game_manager.loop();
    iso::exitApplication(0);
}

