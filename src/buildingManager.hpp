#ifndef _BUILDING_MANAGER_HPP_
#define _BUILDING_MANAGER_HPP_

#include "simulationManager.hpp"
#include "building.hpp"
#include "resourceCost.hpp"

const auto VECTOR0X0         = sf::Vector2f(0, 0);
const auto VECTOR1X1         = sf::Vector2f(1, 1);
const auto BUILDINGSIZE64X64 = sf::Vector2f(64, 64);

namespace iso {
    const Building BUILDING_EMPTY      (VECTOR0X0, VECTOR0X0, VECTOR0X0        , "default"                 , "building_empty"          , VECTOR0X0, ResourceCost(0, 0, 0));
    const Building BUILDING_HOUSE_SMALL(VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_primitive_house", "building_primitive_house", VECTOR1X1, ResourceCost(0, 0, 0));
    const Building BUILDING_FARMLAND   (VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_farmland"       , "building_farmland"       , VECTOR1X1, ResourceCost(0, 0, 0));
    const Building BUILDING_QUARRY     (VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_quarry"         , "building_quarry"         , VECTOR1X1, ResourceCost(0, 0, 0));
    const Building BUILDING_WOODCUTTER (VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_woodcutter"     , "building_woodcutter"     , VECTOR1X1, ResourceCost(0, 0, 0));

    const std::vector <Building> BUILDING_LOOKUP_TABLE = {
        BUILDING_EMPTY,
        BUILDING_HOUSE_SMALL,
        BUILDING_FARMLAND,
        BUILDING_QUARRY,
        BUILDING_WOODCUTTER
    };

    /*  BuildingManager is a tool for placing buildings in the game world.
     *  It does not manage them, update them etc. */
    class BuildingManager {
        private:
            SimulationManager* manager;

        private:
            void placeBuilding(int index, Building building) const;

        public:
            BuildingManager();
            BuildingManager(SimulationManager* manager);
            ~BuildingManager();    

            bool isPositionValid(int index, Building building)    const;
            bool placeBuildingCheck(int index, Building building) const;
            void removeBuilding(int index)                        const;
    };
}

#endif