#include "building.hpp"
#include "region.hpp"
#include "luadriver.hpp"
#include "globalutilities.hpp"

#include <iostream>
#include <fstream>

using namespace iso;

Building::Building() 
    : GameObject(sf::Vector3f(), sf::Vector3f(), sf::Vector2f(), "default", "Building")
{
    this->data.name = "Empty";
}

Building::Building(const Building& building) 
    : GameObject(building) 
{
    this->data = building.data;
}

Building::Building(const BuildingData& data)
    : GameObject(sf::Vector3f(), sf::Vector3f(), sf::Vector2f(data.texture_size), data.texture)
{
    this->data = data;
}

Building::~Building() 
{}

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

const std::string& Building::getDefinitionFilename() const {
    return this->data.filename;
}

const std::string& Building::getBuildingName() const {
    return this->data.name;
}

const std::string& Building::getBuildingDescription() const {
    return this->data.description;
}

const sf::Vector2i Building::getBuildingArea() const {
    return this->data.size;
}

const sf::Vector2i Building::getBuildingScanArea() const {
    return this->data.scan_area;
}

const ResourceList& Building::getBuildingCost() const {
    return this->data.cost;
}

const ResourceList& Building::getBuildingRefund() const {
    return this->data.refund;
}

const std::string& Building::getBuildingIcon() const {
    return this->data.icon_texture;
}

const sf::Vector2i Building::getBuildingIconSize() const {
    return this->data.icon_size;
}

bool Building::isRemovable() const {
    return this->data.removable;
}

const std::vector<BuildingHarvest>& Building::getBuildingHarvests() const {
    return this->data.harvests;
}

const std::vector<BuildingProduction>& Building::getBuildingProduction() const { 
    return this->data.produces;
}

bool Building::isProductionBuilding() const {
    return this->data.produces.size();
}

bool Building::isHarvestBuilding() const {
    return this->data.harvests.size();
}

bool Building::operator== (const Building& building) const {
    return this->getBuildingName() == building.getBuildingName();
}

bool Building::operator!= (const Building& building) const {
    return !(*this == building);  
}

bool Building::isTileHarvestable(GameObject* object, int index) const {
    if(!this->isHarvestBuilding())
        return false;

    auto* region = static_cast<Region*>(object); 
    if(!region->resourceExistsAt(index))
        return false;

    const auto& resource = region->getResourceAt(index);
    for(const auto& item : this->data.harvests) {
        if(item.name == resource.getResourceName())
            return true;
    }

    return false;
}

void Building::update(GameObject* object, int index) const {
    
}

void Building::setBuildingName(const std::string& name) {
    this->data.name = name;
}
