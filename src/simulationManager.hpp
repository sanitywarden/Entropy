#ifndef _SIMULATION_MANAGER_HPP_
#define _SIMULATION_MANAGER_HPP_

#include "entropy/entropy.hpp"
#include "player.hpp"
#include "colours.hpp"

#include "worldGenerator.hpp"
#include "generationSettings.hpp"

#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <queue>

namespace iso {    
    typedef std::map <std::string, std::pair <int, int>> Scheduler;
    typedef std::pair <int, int> ScheduledUpdate;

    /* This table is used to find the position of a texture inside the tile atlas. 
     * Only textures that are used in sprite batching are here. */
    static std::map <std::string, sf::Vector2f> REGIONMAP_TEXTURE_LOOKUP_TABLE = {
        { "default", sf::Vector2f(0, 0) },

        { "tile_grass_warm"            , sf::Vector2f(0, 0)     }, 
        { "tile_grass_cold"            , sf::Vector2f(64, 0)    }, 
        { "tile_grass_subtropical"     , sf::Vector2f(128, 0)   }, 
        { "tile_grass_tropical"        , sf::Vector2f(192, 0)   }, 
        { "tile_tundra"                , sf::Vector2f(256, 0)   }, 
        { "tile_arctic"                , sf::Vector2f(320, 0)   }, 
        { "tile_desert"                , sf::Vector2f(384, 0)   }, 
        { "tile_ocean"                 , sf::Vector2f(448, 0)   }, 
        { "tile_sea"                   , sf::Vector2f(512, 0)   },
        { "tile_height_dirt"           , sf::Vector2f(0, 32)    },
        { "tile_height_stone"          , sf::Vector2f(64, 32)   },
        { "tile_resource_stone"        , sf::Vector2f(0, 64)    },
        { "tile_tree_warm1"            , sf::Vector2f(0, 0)     },
        { "tile_tree_warm2"            , sf::Vector2f(64, 0)    },
        { "tile_tree_cold1"            , sf::Vector2f(128, 0)   },
        { "tile_tree_tropical1"        , sf::Vector2f(256, 0)   },
    };

    class SimulationManager : public entropy::Entropy {
        private:
            void internalLoop(float delta_time);
            void spawnPlayers();

            void updateScheduler();
            void updateBuildings();

        private:
            Scheduler global_updates;
        
        public:
            GenerationSettings   settings;
            WorldGenerator       world;
            std::vector <Player> players;
            int                  time;

        public:
            SimulationManager();
            ~SimulationManager();

            Player& getHumanPlayer();
            std::vector <int> astar(int start, int end) const;

            void w_astar(int start, int end) const;

            void prepare();
            void loop() override;
            std::string getInGameDateFormatted() const;
    };
}

#endif