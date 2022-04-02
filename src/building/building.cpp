#include "building.hpp"
#include <iostream>

using namespace iso;

Building::Building() : GameObject(VECTOR0X0, VECTOR0X0, VECTOR0X0, "*") {
    this->object_name    = "Empty";
    this->building_size  = VECTOR0X0;
    this->building_cost  = ResourceCollection(0, 0, 0);
    this->numerical_type = 0;
}

Building::Building(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name, std::string building_name, int numerical_type, sf::Vector2f building_size, ResourceCollection building_cost) 
    : GameObject(position, relative_position, size, texture_name) 
{
    this->object_name         = building_name;
    this->building_size       = building_size;
    this->building_cost       = building_cost;
    this->numerical_type      = numerical_type;
}

Building::Building(const Building& building) 
    : GameObject(building) 
{
    this->object_name         = building.getName();
    this->building_size       = building.getBuildingArea();
    this->building_cost       = building.getBuildingCost();
    this->numerical_type      = building.getNumericalType();
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

void Building::setGenerationSettings(const GenerationSettings& settings) {
    this->generation_settings = settings;
}

bool Building::operator== (const Building& building) {
    return this->numerical_type == building.getNumericalType();
}

bool Building::operator!= (const Building& building) {
    return !(*this == building);  
}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray game_object(sf::Quads, 4);

    game_object[0].position = this->object_position;    
    game_object[1].position = this->object_position + sf::Vector2f(this->object_size.x, 0);
    game_object[2].position = this->object_position + sf::Vector2f(this->object_size.x, this->object_size.y);
    game_object[3].position = this->object_position + sf::Vector2f(0, this->object_size.y);

    game_object[0].texCoords = sf::Vector2f(0, 0);
    game_object[1].texCoords = sf::Vector2f(this->object_size.x, 0);
    game_object[2].texCoords = sf::Vector2f(this->object_size.x, this->object_size.y);
    game_object[3].texCoords = sf::Vector2f(0, this->object_size.y);
    
    target.draw(game_object, states);
}

void Building::assignBuildingProperties(const Building& building) {
    this->numerical_type = building.numerical_type;
    this->building_size  = building.building_size;
    this->building_cost  = building.building_cost;
}

void Building::assignAllProperties(const Building& building) {
    this->assignBuildingProperties(building);

    this->object_name         = building.getName();
    this->object_position     = building.getPosition();
    this->object_size         = building.getSize();
    this->object_texture_name = building.getTextureName();
    this->object_colour       = building.getColour();
}