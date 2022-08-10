#include "simulationManager.hpp"
#include "generationSettings.hpp"
#include "building.hpp"
#include "resource.hpp"
#include "luadriver.hpp"
#include "item.hpp"
#include "biome.hpp"

// Define the 'extern' variables here.

iso::GameSettings game_settings;
std::vector <iso::Building> buildings;
std::vector <iso::Resource> resources;
std::vector <iso::StorageItem> items;
std::vector <iso::Biome> biomes;

// Leave the lua driver to be initialised last, 
// as it requires all other extern variables to be declared.

lua::driver::Driver lua_driver;

std::vector <std::string> event_queue;

int main() {
    static iso::SimulationManager game_manager;
    game_manager.loop();
    game_manager.exitApplication(0);
}

