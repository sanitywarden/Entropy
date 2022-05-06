#ifndef _BUILDING_DEFINITIONS_HPP_
#define _BUILDING_DEFINITIONS_HPP_

#include "building.hpp"
#include "path_dirt.hpp"
#include "path_stone.hpp"
#include "house_small.hpp"
#include "gatherer.hpp"
#include "quarry.hpp"
#include "woodcutter.hpp"
#include "hunter.hpp"
#include "animal_spot.hpp"
#include "flint_collector.hpp"

#include <vector>

namespace iso {
    const Building       BUILDING_EMPTY;           // ID 0.
    const PathDirt       BUILDING_PATH_DIRT;       // ID 1.
    const PathStone      BUILDING_PATH_STONE;      // ID 2.
    const HouseSmall     BUILDING_HOUSE_SMALL;     // ID 3.
    const Farmhouse      BUILDING_FARM;            // ID 4.
    const Quarry         BUILDING_QUARRY;          // ID 5.
    const Woodcutter     BUILDING_WOODCUTTER;      // ID 6.
    const Hunter         BUILDING_HUNTER;          // ID 7.
    const AnimalSpot     BUILDING_ANIMAL_SPOT;     // ID 8. Not listed, world generation specific building.
    const FlintCollector BUILDING_FLINT_COLLECTOR; // ID 9.

    // If the building should be possible to place, add it here.
    const static std::vector <Building> BUILDING_LOOKUP_TABLE = {
        BUILDING_EMPTY,
        BUILDING_PATH_DIRT,
        BUILDING_PATH_STONE,
        BUILDING_FARM,
        BUILDING_QUARRY,
        BUILDING_WOODCUTTER,
        BUILDING_HOUSE_SMALL,
        BUILDING_HUNTER,
        BUILDING_FLINT_COLLECTOR,
    };
}

#endif