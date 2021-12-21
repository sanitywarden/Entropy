#include "building.hpp"

using namespace iso;

Building::Building() {
    
}

Building::Building(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name, std::string building_name, sf::Vector2f building_size) {
    this->object_name         = building_name;
    this->object_position     = position + relative_position;
    this->object_size         = size;
    this->object_texture_name = texture_name;

    this->building_size = building_size;
}

Building::~Building() {
    
}