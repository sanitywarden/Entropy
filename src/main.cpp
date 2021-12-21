#include "entropy/entropy.hpp"
#include "worldmap.hpp"
#include "simulationManager.hpp"

int main() {
    static iso::SimulationManager game_manager;

    try {
        game_manager.loop();
    } catch(const std::exception& exception) {
        game_manager.exitApplication(1);
        std::cout << exception.what() << "\n";
    }
}

