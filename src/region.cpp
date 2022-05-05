#include "region.hpp"
#include "./building/building_definitions.hpp"
#include "globalutilities.hpp"
#include "generationSettings.hpp"

#include <iostream>

using namespace iso;

Region::Region() 
    : GameObject(VECTOR0X0X0, VECTOR0X0X0, VECTOR0X0, "*", "Region") 
{
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

RiverDirection Region::riverDirection() {
    return this->_direction;
}

bool Region::isOwned() const {
    return this->owner != nullptr;
}

void Region::addResource(Resource resource) {
    auto resource_name     = resource.getName();
    auto resource_quantity = resource.getQuantity();
    
    if(!this->resources.count(resource_name))
        this->resources[resource_name] = resource_quantity;

    else
        this->resources[resource_name] += resource_quantity;
}

int Region::getResourceQuantity(Resource resource) const {
    auto resource_name = resource.getName();

    if(this->resources.count(resource_name))
        return this->resources.at(resource_name);
    return 0;
}

bool Region::isBuildingAffordable(const Building& building) const {
    return true;
    // return (this->resources >= building.getBuildingCost());
}

/* Remove the resources needed to construct provided building from region's resource pool. */
void Region::removeBuildingCost(const Building& building) {
    // this->resources -= building.getBuildingCost();
}

bool Region::isPositionValid(const Building& building, int index) const {
    const auto building_size             = building.getBuildingArea();
    const auto foundation_tile_elevation = this->map[index].getElevation(); 

    for(int y = 0; y < building_size.y; y++) {
        for(int x = 0; x < building_size.x; x++) {
            const int i = index + world_settings.calculateRegionIndex(x, y);

            if(this->map.at(i).getElevation() != foundation_tile_elevation) {
                return false;
            }

            if(!this->map.at(i).tiletype.is_terrain()) {
                return false;
            }

            if(this->trees.count(i)) {
                return false;
            }

            if(this->buildings.count(i)) {
                return false;
            }
        }
    }

    return true;
}

void Region::placeBuilding(Building building, sf::Vector2f texture_size, int index) {
    const Tile& tile         = this->map[index];
    building.object_position = tile.getPosition();

    const int a1_w = 0; 
    const int a1_h = world_settings.tileSize().y; 
    const int r_w  = world_settings.tileSize().x / 2;
    const int r_h  = world_settings.tileSize().y;    
    const int n    = building.getBuildingArea().x;                    
    
    // Here you adjust the origin of buildings with sizes of n > 0.
    // Texture size scale are arithmetic series.
    auto offset = sf::Vector3f(0, 0, 0);  
    if(n > 0) {
        offset = sf::Vector3f(
            -(a1_w + (n - 1) * r_w),
            -(a1_h + (n - 1) * r_h),
            0
        );
    }

    building.object_position += offset;

    std::shared_ptr <Building> sp_building = nullptr;
    if(building == BUILDING_PATH_DIRT)
        sp_building = std::shared_ptr <Building> (new PathDirt());

    else if(building == BUILDING_PATH_STONE)
        sp_building = std::shared_ptr <Building> (new PathStone());

    else if(building == BUILDING_HOUSE_SMALL) 
        sp_building = std::shared_ptr <Building> (new HouseSmall());

    else if(building == BUILDING_FARM) 
        sp_building = std::shared_ptr <Building> (new Farmhouse());

    else if(building == BUILDING_QUARRY)
        sp_building = std::shared_ptr <Building> (new Quarry());

    else if(building == BUILDING_WOODCUTTER) 
        sp_building = std::shared_ptr <Building> (new Woodcutter());
    
    else if(building == BUILDING_HUNTER)
        sp_building = std::shared_ptr <Building> (new Hunter());

    else if(building == BUILDING_ANIMAL_SPOT)
        sp_building = std::shared_ptr <Building> (new AnimalSpot());

    else if(building == BUILDING_FLINT_COLLECTOR)
        sp_building = std::shared_ptr <Building> (new FlintCollector());

    for(int y = 0; y < building.getBuildingArea().y; y++) {
        for(int x = 0; x < building.getBuildingArea().x; x++) {
            const int i = index + world_settings.calculateRegionIndex(x, y);

            // This could be done better.
            // This is a filler so that when a building is bigger than 1x1,
            // other building can not be placed on the building's adjacent tiles.
            this->buildings[i] = std::shared_ptr <Building> (new Building());
        }
    }
    
    sp_building.get()->assignAllProperties(building);
    this->buildings[index] = sp_building;
}

bool Region::placeBuildingCheck(Building building, sf::Vector2f texture_size, int index) {
    if(this->isPositionValid(building, index) && this->isBuildingAffordable(building)) {
        this->placeBuilding(building, texture_size, index);
        this->removeBuildingCost(building);
        return true;
    }

    return false;
}

void Region::removeBuilding(int index) {
    if(this->buildings.count(index)) {
        auto building   = this->buildings.at(index).get();
        
        // this->resources += building->getBuildingRefund();
        
        for(int y = 0; y < building->getBuildingArea().y; y++) {
            for(int x = 0; x < building->getBuildingArea().x; x++) {
                const int i = index + world_settings.calculateRegionIndex(x, y);
                this->buildings.erase(i);
            }
        }
    }
}

bool Region::isUnitPresent() const {
    return this->unit != nullptr;
}

bool Region::isTree(int index) const {
    return this->trees.count(index);
}

Building* Region::getBuildingAt(int index) const {
    if(this->buildings.count(index))
        return this->buildings.at(index).get();
    return nullptr;    
}   

// Returns the number of buildings buildings in proximity.
// Proximity is understood as the building's production area.
int Region::isBuildingInProximity(const Building& building, int building_index) const {
    const auto search_area = building.getProductionArea();
    int buildings_in_proximity = 0;

    for(int y = -search_area.y; y <= search_area.y; y++) {
        for(int x = -search_area.x; x <= search_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);

            // Building or a nullptr.
            auto tile_building = this->getBuildingAt(index);
            if(tile_building) {
                // If the building on this tile is the same building type.
                if(tile_building->getName() == building.getName())
                    buildings_in_proximity++; 
            }
        }
    } 

    return buildings_in_proximity;
}

bool Region::isPath(int index) const {
    auto* building = this->getBuildingAt(index);

    if(building) {
        const auto& texture_name = building->getTextureName();
        return startsWith(texture_name, "path");
    }

    return false;
}