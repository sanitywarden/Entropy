#ifndef _SIMULATION_MANAGER_HPP_
#define _SIMULATION_MANAGER_HPP_

#include "entropy/entropy.hpp"
#include "player.hpp"
#include "colours.hpp"
#include "worldGenerator.hpp"
#include "texturizer.hpp"
#include "noiseSettings.hpp"

#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <queue>

namespace iso { 
    typedef std::map <std::string, std::pair <int, int>> Scheduler;
    typedef std::pair <int, int> ScheduledUpdate;

    class SimulationManager : public entropy::Entropy {
        private:
            void internalLoop(float delta_time);

            void initialise();
            void initialiseWorld();

            void updateScheduler();
            void updateBuildings();
            void updateUnits();
        private:
            int draw_calls;

            Scheduler global_updates;

        public:
            Texturizer texturizer;

            WorldGenerator       world;
            std::vector <Player> players;
            int                  time;    // Time passed since game started in real time seconds.

        public:
            SimulationManager();
            ~SimulationManager();

            void updateDrawCalls(int calls);
            int  getDrawCalls() const;
            bool inScreenSpace(const GameObject& object) const;

            std::string getDateFormatted() const;
            
            bool isHumanPlayer(int player_id) const;
            Player* getHumanPlayer();
            Player* getPlayer(int player_id);
            Unit* getUnit(int unit_id);

            std::vector <int> astar(int start, int end) const;
            void prepare();
            void loop() override;
    };
}

#endif