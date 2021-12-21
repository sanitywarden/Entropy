#ifndef _BUILDING_MANAGER_HPP_
#define _BUILDING_MANAGER_HPP_

#include "building.hpp"
#include "simulationManager.hpp"

const auto VECTOR0X0           = sf::Vector2f(0, 0);
const auto VECTOR1X1           = sf::Vector2f(1, 1);
const auto VECTOR2X2           = sf::Vector2f(2, 2);
const auto BUILDINGSIZE64X64   = sf::Vector2f(64, 64);
const auto BUILDINGSIZE128X128 = sf::Vector2f(128, 128);
const auto BUILDINGSIZE128X256 = sf::Vector2f(128, 256);

namespace iso {
    const Building BUILDING_TREECUTTER (VECTOR0X0, VECTOR0X0, BUILDINGSIZE128X256, "building_treecutter" , "building_treecutter" , VECTOR2X2);
    const Building BUILDING_STONECUTTER(VECTOR0X0, VECTOR0X0, BUILDINGSIZE128X256, "building_stonecutter", "building_stonecutter", VECTOR2X2);
    const Building BUILDING_HOUSE_SMALL(VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64  , "building_house_small", "building_house_small", VECTOR1X1);

    /*  BuildingManager is a tool for placing buildings in the game world.
     *  It does not manage them, update them etc. */
    class BuildingManager {
        private:
            SimulationManager* manager;

        public:
            BuildingManager();
            BuildingManager(SimulationManager* manager);
            ~BuildingManager();    

            bool isPositionValid(int index, Building building)    const;
            void placeBuilding(int index, Building building)      const;
            bool placeBuildingCheck(int index, Building building) const;
            void removeBuilding(int index)                        const;
    };
}

#endif