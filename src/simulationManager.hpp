#ifndef _SIMULATION_MANAGER_HPP_
#define _SIMULATION_MANAGER_HPP_

#include "entropy/entropy.hpp"
#include "player.hpp"
#include "worldGenerator.hpp"
#include "texturizer.hpp"
#include "noiseSettings.hpp"
#include "luadriver.hpp"

#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <queue>

namespace iso { 
    typedef std::map <std::string, std::pair <int, int>> Scheduler;
    typedef std::pair <int, int> ScheduledUpdate;

    constexpr int seconds_per_hour  = 12;
    constexpr int seconds_per_day   = seconds_per_hour  * 12;
    constexpr int seconds_per_week  = seconds_per_day   * 7;
    constexpr int seconds_per_month = seconds_per_day   * 30;
    constexpr int seconds_per_year  = seconds_per_month * 12; 

    constexpr int food_consumed_per_pop  = 1;
    constexpr int water_consumed_per_pop = 2; 

    class SimulationManager : public Entropy {
        private:
            void initialise();
            void internalLoop(float delta_time);

            void generateCountries();

            void updateSchedulerGlobal();
            void updateShedulerSimulation();
            void updateBuildings();
            void updateUnits();
            void updatePopulation();
            void updateRandomEvent();
            // void updateQuest();
        private:
            int draw_calls;
            int people_dehydrated;   // Number of people with water needs not satisfied.
            int people_malnourished; // Number of people with food needs not satisfied.

            float simulation_speed;                // Simulation speed. Events, updates and other gameplay-related stuff depend on this. 1 is default value, and equals to updating every 1 second.             
            sf::Clock simulation_clock;            // For timing simulation updates internally. Use this instead of Entropy's clock (it's intended for measuring FPS).
            sf::Time  simulation_time_since_start; // Time since clock start.      

            Scheduler global_updates;
            Scheduler simulation_updates;

        public:
            Texturizer texturizer;

            WorldGenerator       world;
            std::vector <Player> players;
            int                  time;            // Time passed since game started in real time seconds.
            int                  simulation_time; // Time passed inside the simulation. Depends on the frequency of updates.
            
            // TODO: move somewhere else.
            int font_size;
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

            void emitEvents();
            void popEvents();

            // Search for the requested unit in all players' armies.
            // If not found return nullptr.
            // Unit* getUnit(int unit_id);
            
            // Creates a unit of certain type.
            // The unit will be created if the specified region is able to support more units.
            // The created unit has all required properties of the class.
            // std::shared_ptr <Unit> createUnit(UnitType unit_type, int region_index, Player* owner) const;
            
            // Deletes all existing data about the unit.
            // Removes the unit pointer in the region (as the unit no longer stands there).
            // Removes the unit from the player's army (from the vector of units in iso::Player class).
            // Sets the selected_unit_id in iso::Worldmap to -1.
            // void deleteUnit(int unit_id); 

            bool regionCanBeColonised(int region_index) const;

            // A* algorithm to find the shortest path in the Worldmap stage.
            std::vector <int> astar(int start, int end) const;

            // A* algorithm to find the shortest path in the Regionmap stage.
            std::vector <int> r_astar(int start, int end) const;

            void prepare();
            void loop() override;

            void generateWorld();
    };
}

#endif