#include "building.hpp"
#include <iostream>

using namespace iso;

Building::Building() : GameObject(VECTOR0X0, VECTOR0X0, VECTOR0X0, "*") {
    this->object_name    = "Empty";
    this->building_size  = VECTOR0X0;
    this->building_cost  = ResourceCollection(0, 0, 0);
    this->numerical_type = 0;
}

Building::Building(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name, std::string building_name, int numerical_type, sf::Vector2f building_size, ResourceCollection building_cost) : GameObject(position, relative_position, size, texture_name) {
    this->object_name    = building_name;
    this->building_size  = building_size;
    this->building_cost  = building_cost;
    this->numerical_type = numerical_type;
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

const int Building::getNumericalType() const {
    return this->numerical_type;
}

bool Building::operator== (const Building& building) {
    return this->numerical_type == building.getNumericalType();
}

bool Building::operator!= (const Building& building) {
    return !(*this == building);  
}
