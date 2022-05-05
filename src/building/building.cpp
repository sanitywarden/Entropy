#include "building.hpp"

#include <iostream>

using namespace iso;

Building::Building() 
    : GameObject(VECTOR0X0X0, VECTOR0X0X0, VECTOR0X0, "*", "Building") 
{
    this->building_menu_icon      = "default";
    this->building_size           = VECTOR0X0;
    this->numerical_type          = 0;
    this->building_proximity_area = VECTOR0X0;
    this->building_name           = "Empty";
}

Building::Building(sf::Vector2f size, std::string texture_name, std::string building_name, std::string building_menu_icon, int numerical_type, sf::Vector2f building_size, sf::Vector2f proximity) 
    : GameObject(VECTOR0X0X0, VECTOR0X0X0, size, texture_name, "Building") 
{
    this->building_menu_icon      = building_menu_icon;
    this->building_size           = building_size;
    this->numerical_type          = numerical_type;
    this->building_proximity_area = proximity;
    this->building_name           = building_name;
}

Building::Building(const Building& building) 
    : GameObject(building) 
{
    this->building_menu_icon      = building.getBuildingMenuIconName();
    this->building_size           = building.getBuildingArea();
    this->numerical_type          = building.getNumericalType();
    this->building_proximity_area = building.getProductionArea();
    this->building_name           = building.getBuildingName();
}

Building::~Building() {
    
}

const sf::Vector2f Building::getBuildingArea() const {
    return this->building_size;
}

const sf::Vector2f Building::getProductionArea() const {
    return this->building_proximity_area;
}

// const ResourceCollection Building::getBuildingCost() const {
//     return this->building_cost;
// }

// const ResourceCollection Building::getBuildingRefund() const {
    // return ResourceCollection(
        // this->building_cost.wood  * 0.9f,
        // this->building_cost.stone * 0.9f,
        // this->building_cost.gold  * 0.9f
    // );
// }

const int Building::getNumericalType() const {
    return this->numerical_type;
}

bool Building::operator== (const Building& building) const {
    return this->numerical_type == building.getNumericalType();
}

bool Building::operator!= (const Building& building) const {
    return !(*this == building);  
}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray game_object(sf::Quads, 4);

    auto position2d = this->getPosition2D();
    auto size       = this->getSize();

    game_object[0].position = position2d;    
    game_object[1].position = position2d + sf::Vector2f(size.x, 0);
    game_object[2].position = position2d + sf::Vector2f(size.x, size.y);
    game_object[3].position = position2d + sf::Vector2f(0, size.y);

    game_object[0].texCoords = sf::Vector2f(0, 0);
    game_object[1].texCoords = sf::Vector2f(size.x, 0);
    game_object[2].texCoords = sf::Vector2f(size.x, size.y);
    game_object[3].texCoords = sf::Vector2f(0, size.y);
    
    target.draw(game_object, states);
}

void Building::assignBuildingProperties(const Building& building) {
    this->numerical_type = building.numerical_type;
    this->building_size  = building.building_size;
}

void Building::assignAllProperties(const Building& building) {
    this->assignBuildingProperties(building);

    this->object_name         = building.getName();
    this->object_position     = building.getPosition();
    this->object_size         = building.getSize();
    this->object_texture_name = building.getTextureName();
    this->object_colour       = building.getColour();
}

std::string Building::getBuildingMenuIconName() const {
    return this->building_menu_icon;
}

std::string Building::getBuildingName() const {
    return this->building_name;
}