#include "simulationManager.hpp"

int main() {
    static iso::SimulationManager game_manager;
    game_manager.loop();
    game_manager.exitApplication(0);
}

