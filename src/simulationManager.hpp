#ifndef _SIMULATION_MANAGER_HPP_
#define _SIMULATION_MANAGER_HPP_

#include "entropy/entropy.hpp"
#include "player.hpp"

#include "worldGenerator.hpp"
#include "generationSettings.hpp"

#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <queue>

namespace iso {    
    struct UpdateUtility {
        std::string id;
        int counter;  // Counter. The units do not matter (can be seconds, milliseconds etc.).
        int interval; // How often is the update executed?
        bool updated;

        UpdateUtility() : id("*"), counter(0), interval(0), updated(false) {};
        UpdateUtility(std::string id, int interval) : id(id), counter(0), interval(interval), updated(false) {};

        bool isReady() {
            return this->counter % this->interval == 0;
        }

        void resetCounter() {
            this->counter = 0;
        }

        void setInterval(int interval) {
            this->interval = interval;
        }
    };

    class SimulationManager : public entropy::Entropy {
        private:
            void internalLoop(float delta_time);
            void spawnPlayers();
        public:
            GenerationSettings   settings;
            WorldGenerator       world;
            std::vector <Player> players;
            int                  time_passed; // Time passed since start of the program. In seconds.
            int                  average_fps;
        public:
            SimulationManager();
            ~SimulationManager();

            int getAverageFramesPerSecond();

            Player& getHumanPlayer();
            std::vector <int> astar(int start, int end);

            void prepare();
            void loop() override;
    };
}

#endif