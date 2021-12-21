#ifndef _BUILDING_HPP_
#define _BUILDING_HPP_

#include "gameObject.hpp"

namespace iso {
    class Building : public GameObject {
        friend class BuildingManager;
        
        protected:
            sf::Vector2f building_size; // Area occupied by the building, represented by tiles (2x2, 4x4).

        public:
            Building();
            Building(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name, std::string building_name, sf::Vector2f building_size);
            ~Building();
    };
}

#endif