#ifndef _PLAYER_MANAGER_HPP_
#define _PLAYER_MANAGER_HPP_

#include "entropy/entropy.hpp"
#include "player.hpp"

#include "worldGenerator.hpp"
#include "generationSettings.hpp"

#include <SFML/System.hpp>
#include <vector>
#include <string>

namespace iso {
    const int TREE_WOOD_HARVEST = 10; // Amount of wood granted for cutting a tree. 

    class SimulationManager : public entropy::Entropy {
        private:
            void internalLoop(float delta_time);
            void spawnPlayers();

            // void regionmapUpdateTile(Player& player);
            // void regionmapPlaceBuilding(Player& player);

        public:
            worldGenerator        world;
            WorldSettings         world_settings;
            RegionSettings        region_settings;
            std::vector <Player>  players;
            
        public:
            SimulationManager();
            ~SimulationManager();

            void loop() override;
    };
}

#endif