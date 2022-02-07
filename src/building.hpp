#ifndef _BUILDING_HPP_
#define _BUILDING_HPP_

#include "gameObject.hpp"
#include "resources.hpp"

const auto VECTOR0X0         = sf::Vector2f(0, 0);
const auto VECTOR1X1         = sf::Vector2f(1, 1);
const auto BUILDINGSIZE64X64 = sf::Vector2f(64, 64);
const auto BUILDINGSIZE64X32 = sf::Vector2f(64, 32);

namespace iso {
    class Building : public GameObject {
        friend class BuildingManager;
        
        protected:
            int                numerical_type;
            sf::Vector2f       building_size;
            ResourceCollection building_cost;

        public:
            Building();
            Building(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name, std::string building_name, int numerical_type, sf::Vector2f building_size, ResourceCollection building_cost);
            ~Building();

            /* Get the area occupied by the building. */
            const sf::Vector2f getBuildingArea() const;

            /* Get the cost of the building. */
            const ResourceCollection getBuildingCost()   const;

            /* Get the refund of the building (when the building is destroyed). */
            const ResourceCollection getBuildingRefund() const;

            const int getNumericalType() const;

            bool operator == (const Building& building);      
            bool operator != (const Building& building);
    };

    const Building BUILDING_EMPTY      (VECTOR0X0, VECTOR0X0, VECTOR0X0        , "default"                 , "Empty"          , 0, VECTOR0X0, ResourceCollection(0, 0, 0));
    const Building BUILDING_HOUSE_SMALL(VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_small_house"    , "Small House"    , 1, VECTOR1X1, ResourceCollection(0, 0, 0));
    const Building BUILDING_FARMLAND   (VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_farmland"       , "Farmhouse"      , 2, VECTOR1X1, ResourceCollection(0, 0, 0));
    const Building BUILDING_QUARRY     (VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_quarry"         , "Quarry"         , 3, VECTOR1X1, ResourceCollection(0, 0, 0));
    const Building BUILDING_WOODCUTTER (VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_woodcutter"     , "Woodcutter"     , 4, VECTOR1X1, ResourceCollection(0, 0, 0));
    const Building BUILDING_HOUSE_TENT (VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_primitive_house", "Primitive House", 5, VECTOR1X1, ResourceCollection(0, 0, 0));
    const Building BUILDING_PATH_DIRT  (VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X32, "path_dirt_point"         , "Dirt path"      , 6, VECTOR1X1, ResourceCollection(0, 0, 0));
    const Building BUILDING_PATH_STONE (VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X32, "path_stone_point"        , "Stone path"     , 7, VECTOR1X1, ResourceCollection(0, 0, 0));

    static const std::vector <Building> BUILDING_LOOKUP_TABLE = {
        BUILDING_HOUSE_SMALL,
        BUILDING_FARMLAND,
        BUILDING_QUARRY,
        BUILDING_WOODCUTTER,
        BUILDING_HOUSE_TENT,
        BUILDING_PATH_DIRT,
        BUILDING_PATH_STONE
    };
}

#endif