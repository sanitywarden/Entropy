#ifndef _SIMULATION_MANAGER_HPP_
#define _SIMULATION_MANAGER_HPP_

#include "entropy/entropy.hpp"
#include "player.hpp"

#include "worldGenerator.hpp"
#include "generationSettings.hpp"

#include <SFML/System.hpp>
#include <vector>
#include <string>

namespace iso {
    class SimulationManager : public entropy::Entropy {
        private:
            void internalLoop(float delta_time);
            void spawnPlayers();

        public:
            GenerationSettings   settings;
            worldGenerator       world;
            std::vector <Player> players;

        public:
            Player& getHumanPlayer();

        public:
            SimulationManager();
            ~SimulationManager();

            void loop() override;
    };
}

#endif