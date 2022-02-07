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

    class SimulationManager : public entropy::Entropy {
        private:
            bool performChecks();
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