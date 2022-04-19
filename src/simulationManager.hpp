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

    /* This table is used to find the position of a texture inside the tile atlas. 
     * Only textures that are used in sprite batching are here. */
    static std::map <std::string, sf::Vector2f> REGIONMAP_TEXTURE_LOOKUP_TABLE = {
        { "default", sf::Vector2f(0, 0) },

        // Tiles.
        { "tile_grass_warm"            , sf::Vector2f(0, 0)     }, 
        { "tile_grass_cold"            , sf::Vector2f(64, 0)    }, 
        { "tile_grass_subtropical"     , sf::Vector2f(128, 0)   }, 
        { "tile_grass_tropical"        , sf::Vector2f(192, 0)   }, 
        { "tile_tundra"                , sf::Vector2f(256, 0)   }, 
        { "tile_ocean"                 , sf::Vector2f(320, 0)   }, 
        { "tile_river"                 , sf::Vector2f(384, 0)   },
        { "tile_height_dirt"           , sf::Vector2f(0, 32)    },
        { "tile_height_stone"          , sf::Vector2f(64, 32)   },
        
        // Resources.
        { "tile_resource_stone"        , sf::Vector2f(0, 64)    },
        { "tile_resource_flint"        , sf::Vector2f(64, 64)   },

        // Trees.
        // Tundra.
        { "tile_tree_spruce_1", sf::Vector2f(0, 96) },
        { "tile_tree_spruce_2", sf::Vector2f(64, 96)},
        { "tile_tree_pine"   , sf::Vector2f(704, 0)},
        
        // Temperate.
        { "tile_tree_beech", sf::Vector2f(0, 0)    },
        { "tile_tree_oak"  , sf::Vector2f(64, 0)   },
        { "tile_tree_maple", sf::Vector2f(128, 0)  },
        
        // Mediterranean.
        { "tile_tree_cypress", sf::Vector2f(0, 192)},
        { "tile_tree_acacia" , sf::Vector2f(64, 192)},
        
        // Tropical.
        { "tile_tree_palm"   , sf::Vector2f(0, 288)},
    };

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
            int                  time;

        public:
            SimulationManager();
            ~SimulationManager();

            void updateDrawCalls(int calls);
            int  getDrawCalls() const;

            Player& getHumanPlayer();
            bool    isHumanPlayer(int player_id) const;
            Unit*   getUnit(int unit_id);
            std::vector <int> astar(int start, int end) const;

            void prepare();
            void loop() override;
    };
}

#endif