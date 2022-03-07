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
            const ResourceCollection getBuildingCost() const;

            /* Get the refund of the building (when the building is destroyed). */
            const ResourceCollection getBuildingRefund() const;

            const int getNumericalType() const;

            // Equality and inequality operators return a value based on the building ID. 

            bool operator == (const Building& building);      
            bool operator != (const Building& building);
    };
}

#endif