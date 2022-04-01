#include "region.hpp"
#include <iostream>

using namespace iso;

Region::Region() : GameObject(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Vector2f(0, 0), "*") {
    this->object_name = "region";

    this->_marked    = false;
    this->_direction = RiverDirection::RIVER_NONE;
    
    this->height      = 0.0f;
    this->latitude    = 0.0f;
    this->moisture    = 0.0f;
    this->temperature = 0.0f;
    this->visited     = false;
    
    this->owner = nullptr;
    this->unit  = nullptr;

    this->map.resize(0);
    this->population.resize(0);
}

Region::~Region() {

}

// Remember that you can implement fog of war by colouring the regions texture (sf::Color).
void Region::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray worldmap_region(sf::Quads, 4);

    const sf::Vector2f position = this->getPosition();

    worldmap_region[0].position = position;
    worldmap_region[1].position = position + sf::Vector2f(this->getSize().x, 0);
    worldmap_region[2].position = position + sf::Vector2f(this->getSize().x, this->getSize().y);
    worldmap_region[3].position = position + sf::Vector2f(0, this->getSize().y);
    
    worldmap_region[0].texCoords = sf::Vector2f(0, 0);
    worldmap_region[1].texCoords = sf::Vector2f(this->getSize().x, 0);
    worldmap_region[2].texCoords = sf::Vector2f(this->getSize().x, this->getSize().y);
    worldmap_region[3].texCoords = sf::Vector2f(0, this->getSize().y);

    auto colour = this->getColour();
    if(colour != COLOUR_BLACK) {
        worldmap_region[0].color = colour; 
        worldmap_region[1].color = colour;
        worldmap_region[2].color = colour;
        worldmap_region[3].color = colour;
    }

    target.draw(worldmap_region, states);
}

RiverDirection Region::riverDirection() {
    return this->_direction;
}

bool Region::isOwned() const {
    return this->owner != nullptr;
}

void Region::addResource(ResourceType resource, int quantity) {
    switch(resource) {
        default:
            break;
        
        case ResourceType::RESOURCE_WOOD:
            this->resources.wood += quantity;
            break;

        case ResourceType::RESOURCE_STONE:
            this->resources.stone += quantity;
            break;

        case ResourceType::RESOURCE_FOOD:
            this->resources.food += quantity;
            break;

        case ResourceType::RESOURCE_GOLD:
            this->resources.gold += quantity;
            break;
    }
}

void Region::addResources(ResourceCollection resource) {
    this->resources += resource;
}

int Region::getResourceQuantity(ResourceType resource) {
    switch(resource) {
        default:
            return 0;

        case ResourceType::RESOURCE_WOOD:
            return this->resources.wood;

        case ResourceType::RESOURCE_STONE:
            return this->resources.wood;
    
        case ResourceType::RESOURCE_GOLD:
            return this->resources.gold;
    }
}

bool Region::isBuildingAffordable(const Building& building) const {
    return (this->resources >= building.getBuildingCost());
}

/* Remove the resources needed to construct provided building from region's resource pool. */
void Region::removeBuildingCost(const Building& building) {
    this->resources -= building.getBuildingCost();
}

bool Region::isPositionValid(const Building& building, const GenerationSettings& settings, int index) const {
    const auto building_size             = building.getBuildingArea();
    const auto foundation_tile_elevation = this->map[index].elevation; 

    for(int x = 0; x < building_size.x; x++) {
        for(int y = 0; y < building_size.y; y++) {
            const int i = (y * settings.region_size) + index + x;

            if(this->map.at(i).elevation != foundation_tile_elevation)
                return false;

            if(!this->map.at(i).tiletype.is_terrain())
                return false;

            if(this->trees.count(i))
                return false;

            if(this->buildings.count(i))
                return false;
        }
    }

    return true;
}

void Region::placeBuilding(Building building, const GenerationSettings& settings, int index) {
    const Tile& tile         = this->map[index];
    building.object_position = tile.getPosition();

    if(building.getSize().y != 32)
        building.object_position += sf::Vector2f(0, -building.getSize().y / 4 -settings.region_tile_size.y / 2);
    
    std::pair <int, Building> pair(index, building);

    this->buildings.insert(pair);
}

bool Region::placeBuildingCheck(Building building, const GenerationSettings& settings, int index) {
    if(this->isPositionValid(building, settings, index) && this->isBuildingAffordable(building)) {
        this->placeBuilding(building, settings, index);
        this->removeBuildingCost(building);
        return true;
    }

    return false;
}

void Region::removeBuilding(int index) {
    if(this->buildings.count(index)) {
        const Building& building = this->buildings[index];   
        this->resources += building.getBuildingRefund();
        this->buildings.erase(index);
    }
}

bool Region::isUnitPresent() {
    return this->unit != nullptr;
}

bool Region::tileIsTree(int index) const {
    return this->trees.count(index);
}