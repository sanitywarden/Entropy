#include "building.hpp"
#include "region.hpp"
#include "globalutilities.hpp"
#include "colours.hpp"
#include "simulationManager.hpp"

#include <iostream>

namespace iso {
Building::Building() 
    : GameObject(core::Vector3i(0, 0, 0), core::Vector3i(0, 0, 0), core::Vector2i(0, 0), "default", "Building")
{
    this->data.name = "Empty";
}

Building::Building(const Building& building)
    : GameObject(building) 
{
    this->data = building.data;
}

Building::Building(const BuildingData& data)
    : GameObject(core::Vector3i(0, 0, 0), core::Vector3i(0, 0, 0), data.texture_size, data.texture, "Building")
{
    this->data = data;
}

Building::~Building()
{}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(states.texture == nullptr) {
        printError("Building::draw()", "Texture is nullptr");
        return;
    }

    sf::VertexArray game_object(sf::Quads, 4);

    auto position2d = this->getPosition2D().asSFMLVector2f();
    auto size       = this->getSize().asSFMLVector2f();
    auto colour     = this->getColour();

    game_object[0].position = position2d;    
    game_object[1].position = position2d + sf::Vector2f(size.x, 0);
    game_object[2].position = position2d + sf::Vector2f(size.x, size.y);
    game_object[3].position = position2d + sf::Vector2f(0, size.y);

    game_object[0].texCoords = sf::Vector2f(0, 0);
    game_object[1].texCoords = sf::Vector2f(size.x, 0);
    game_object[2].texCoords = sf::Vector2f(size.x, size.y);
    game_object[3].texCoords = sf::Vector2f(0, size.y);

    if(colour != COLOUR_BLACK) {
        game_object[0].color = colour.asSFMLColour();
        game_object[1].color = colour.asSFMLColour();
        game_object[2].color = colour.asSFMLColour();
        game_object[3].color = colour.asSFMLColour();
    }

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

core::Vector2i Building::getBuildingArea() const {
    return this->data.size;
}

const std::string& Building::getBuildingTexture() const {
    return this->data.texture;
}

core::Vector2i Building::getBuildingScanArea() const {
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

const core::Vector2i Building::getBuildingIconSize() const {
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
    return this->data.produces.size() > 0;
}

bool Building::isHarvestBuilding() const {
    return this->data.harvests.size() > 0;
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
    
    if(region->resourceExistsAt(index)) {
        const auto& resource = region->getResourceAt(index);
        for(const auto& item : this->data.harvests) {
            if(item.name == resource.getResourceName())
                return true;
    
        }
    }

    else {
        for(const auto& item : this->data.harvests) {
            if(item.name == "Tree" && region->treeExistsAt(index))
                return true;
        }
    }
    
    return false;
}

void Building::setBuildingName(const std::string& name) {
    this->data.name = name;
}

void Building::setBuildingDescription(const std::string& description) {
    this->data.description = description;
}

void Building::setBuildingArea(core::Vector2i area) {
    this->data.size = area;
}

void Building::setBuildingTexture(const std::string& texture_name) {
    this->object_texture_name = texture_name;
}

void Building::setBuildingScanArea(core::Vector2i scan_area) {
    this->data.scan_area = scan_area;
}

void Building::setBuildingCost(ResourceList list) {
    this->data.cost = list;
    
    ResourceList refund;
    for(auto resource : list) {
        ItemData item;
        item.filename     = resource.getDefinitionFilename();
        item.name         = resource.getItemName();
        item.description  = resource.getItemDescription();
        item.icon_texture = resource.getIconTexture();
        item.icon_size    = resource.getIconTextureSize();
        item.type         = resource.getItemType();
        item.amount       = 0.6f * resource.getAmount();

        StorageItem refund_resource(item);
        refund.push_back(refund_resource);
    }

    this->data.refund = refund;
}

void Building::setBuildingIcon(const std::string& icon) {
    this->data.icon_texture = icon;
}

void Building::setBuildingIconSize(core::Vector2i size) {
    this->data.icon_size = size;
}

void Building::setBuildingHarvests(std::vector <BuildingHarvest> harvest) {
    this->data.harvests = harvest;
}

void Building::setBuildingProduction(std::vector <BuildingProduction> production) {
    this->data.produces = production;
}

void Building::setRemovable(bool removable) {
    this->data.removable = removable;
}
}