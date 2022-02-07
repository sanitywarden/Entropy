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
    /* This table is used to find the position of a texture inside the tile atlas. 
     * Only textures that are used in sprite batching are here. */
    const std::map <std::string, sf::Vector2f> TEXTURE_LOOKUP_TABLE = {
        { "tile_grass_warm"         , sf::Vector2f(0, 0)     }, 
        { "tile_grass_cold"         , sf::Vector2f(64, 0)    }, 
        { "tile_grass_subtropical"  , sf::Vector2f(128, 0)   }, 
        { "tile_grass_tropical"     , sf::Vector2f(192, 0)   }, 
        { "tile_tundra"             , sf::Vector2f(256, 0)   }, 
        { "tile_arctic"             , sf::Vector2f(320, 0)   }, 
        { "tile_desert"             , sf::Vector2f(384, 0)   }, 
        { "tile_ocean"              , sf::Vector2f(448, 0)   }, 
        { "tile_sea"                , sf::Vector2f(512, 0)   },
        { "tile_height_dirt"        , sf::Vector2f(0, 32)    },
        { "tile_height_stone"       , sf::Vector2f(64, 32)   },
        { "tile_tree_warm1"         , sf::Vector2f(0, 0)     },
        { "tile_tree_warm2"         , sf::Vector2f(64, 0)    },
        { "tile_tree_cold1"         , sf::Vector2f(128, 0)   },
        { "tile_tree_tropical1"     , sf::Vector2f(256, 0)   },
    };
    
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
    
    const int INGAME_TIME_PER_HOUR  = 15;                         // n real-time seconds is one in-game hour.
    const int INGAME_TIME_PER_DAY   = INGAME_TIME_PER_HOUR  * 12; // n hours  per in-game day.
    const int INGAME_TIME_PER_MONTH = INGAME_TIME_PER_DAY   * 15; // n days   per in-game month
    const int INGAME_TIME_PER_YEAR  = INGAME_TIME_PER_MONTH * 6;  // n months per in-game year.

    struct WorldTime {
        int time_passed; // The same as seconds, but this value is never reset.
        int seconds;     // Same as time_passed, but this value is reset every INGAME_TIME_PER_HOUR.
        int hours;
        int days;
        int months;
        int years;

        WorldTime() : time_passed(0), seconds(0), hours(0), days(0), months(0), years(0) {}
        
        void calculateInGameDate() {
            this->seconds++;

            if(this->seconds >= INGAME_TIME_PER_HOUR) {
                this->seconds = 0;
                this->hours++;
            }

            if(this->hours * INGAME_TIME_PER_HOUR >= INGAME_TIME_PER_DAY) {
                this->hours = 0;
                this->days++;
            }

            if(this->days * INGAME_TIME_PER_DAY >= INGAME_TIME_PER_MONTH) {
                this->days = 0;
                this->months++;
            }

            if(this->months * INGAME_TIME_PER_MONTH >= INGAME_TIME_PER_YEAR) {
                this->months = 0;
                this->years++;
            }
        }

        std::string getInGameDateFormatted() {
            return ("[" + std::to_string(this->years) + "-" + std::to_string(this->months) + "-" + std::to_string(this->days) + ":" + std::to_string(this->hours) + "-" + std::to_string(this->seconds) + "]");
        }
    };

    

    class SimulationManager : public entropy::Entropy {
        private:
            bool performChecks();
            void internalLoop(float delta_time);
            void spawnPlayers();
        public:
            GenerationSettings   settings;
            WorldGenerator       world;
            WorldTime            time;
            std::vector <Player> players;
            int                  average_fps;
        public:
            SimulationManager();
            ~SimulationManager();

            int getAverageFramesPerSecond();

            Player& getHumanPlayer();
            std::vector <int> astar(int start, int end);

            void prepare();
            void loop() override;
            std::string getInGameDateFormatted() const;
    };
}

#endif