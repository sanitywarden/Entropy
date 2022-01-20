#include "building.hpp"

using namespace iso;

Building::Building() {
    
}

Building::Building(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name, std::string building_name, sf::Vector2f building_size, ResourceCollection building_cost) {
    this->object_name         = building_name;
    this->object_position     = position + relative_position;
    this->object_size         = size;
    this->object_texture_name = texture_name;

    this->building_size = building_size;
    this->building_cost = building_cost;
}

Building::~Building() {
    
}

const sf::Vector2f Building::getBuildingArea() const {
    return this->building_size;
}

const ResourceCollection Building::getBuildingCost() const {
    return this->building_cost;
}

const ResourceCollection Building::getBuildingRefund() const {
    return ResourceCollection(
        this->building_cost.wood  * 0.9f,
        this->building_cost.stone * 0.9f,
        this->building_cost.gold  * 0.9f
    );
}

bool Building::operator== (const Building& building) {
    return this->object_name == building.getName();
}

bool Building::operator!= (const Building& building) {
    return !(*this == building);  
}