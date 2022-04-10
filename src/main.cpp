#include "simulationManager.hpp"
#include "generationSettings.hpp"

iso::WorldData world_settings;

int main() {
    static iso::SimulationManager game_manager;
    game_manager.loop();
    game_manager.exitApplication(0);
}

