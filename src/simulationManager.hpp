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
    typedef std::map <std::string, std::pair<int, int>> Scheduler;

    /* This table is used to find the position of a texture inside the tile atlas. 
     * Only textures that are used in sprite batching are here. */
    static std::map <std::string, sf::Vector2f> TEXTURE_LOOKUP_TABLE = {
        // { "tile_grass_warm"            , sf::Vector2f(0, 0)     }, 
        // { "tile_grass_cold"            , sf::Vector2f(64, 0)    }, 
        // { "tile_grass_subtropical"     , sf::Vector2f(128, 0)   }, 
        // { "tile_grass_tropical"        , sf::Vector2f(192, 0)   }, 
        // { "tile_tundra"                , sf::Vector2f(256, 0)   }, 
        // { "tile_arctic"                , sf::Vector2f(320, 0)   }, 
        // { "tile_desert"                , sf::Vector2f(384, 0)   }, 
        // { "tile_ocean"                 , sf::Vector2f(448, 0)   }, 
        // { "tile_sea"                   , sf::Vector2f(512, 0)   },
        // { "tile_height_dirt"           , sf::Vector2f(0, 32)    },
        // { "tile_height_stone"          , sf::Vector2f(64, 32)   },
        // { "tile_resource_stone"        , sf::Vector2f(0, 64)    },
        // { "tile_tree_warm1"            , sf::Vector2f(0, 0)     },
        // { "tile_tree_warm2"            , sf::Vector2f(64, 0)    },
        // { "tile_tree_cold1"            , sf::Vector2f(128, 0)   },
        // { "tile_tree_tropical1"        , sf::Vector2f(256, 0)   },
        
        { "panel_island"               , sf::Vector2f(0, 1024)  },
        { "panel_full"                 , sf::Vector2f(0, 1152)  },
        { "panel_coast_up"             , sf::Vector2f(0, 0)     },
        { "panel_coast_down"           , sf::Vector2f(0, 128)   },
        { "panel_coast_left"           , sf::Vector2f(0, 256)   },
        { "panel_coast_right"          , sf::Vector2f(0, 384)   },
        { "panel_coast_corner_tl"      , sf::Vector2f(0, 512)   },
        { "panel_coast_corner_bl"      , sf::Vector2f(0, 640)   },
        { "panel_coast_corner_tr"      , sf::Vector2f(0, 768)   },
        { "panel_coast_corner_br"      , sf::Vector2f(0, 896)   },
        { "panel_coast_turn_tl"        , sf::Vector2f(384, 512) },
        { "panel_coast_turn_bl"        , sf::Vector2f(384, 640) },
        { "panel_coast_turn_tr"        , sf::Vector2f(384, 768) },
        { "panel_coast_turn_br"        , sf::Vector2f(384, 896) },
        { "panel_coast_extension_up"   , sf::Vector2f(384, 0)   },
        { "panel_coast_extension_down" , sf::Vector2f(384, 128) },
        { "panel_coast_extension_left" , sf::Vector2f(384, 256) },
        { "panel_coast_extension_right", sf::Vector2f(384, 384) },
        { "*", sf::Vector2f(0, 0) }
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

            void prepare();
            void loop() override;
            std::string getInGameDateFormatted() const;
    };
}

#endif