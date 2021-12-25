#ifndef _BUILDING_HPP_
#define _BUILDING_HPP_

#include "gameObject.hpp"
#include "resourceCost.hpp"

namespace iso {
    class Building : public GameObject {
        friend class BuildingManager;
        
        protected:
            sf::Vector2f building_size;
            ResourceCost building_cost;

        public:
            Building();
            Building(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name, std::string building_name, sf::Vector2f building_size, ResourceCost building_cost);
            ~Building();

            /* Get the area occupied by the building. */
            const sf::Vector2f getBuildingArea() const;

            /* Get the cost of the building. */
            const ResourceCost getBuildingCost()   const;

            /* Get the refund of the building (when the building is destroyed). */
            const ResourceCost getBuildingRefund() const;
            
    };
}

#endif