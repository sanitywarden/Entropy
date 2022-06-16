#include "region.hpp"
#include "./building/building_definitions.hpp"
#include "globalutilities.hpp"
#include "generationSettings.hpp"

#include <iostream>
#include <string>

using namespace iso;

Region::Region() 
    : GameObject(VECTOR0X0X0, VECTOR0X0X0, VECTOR0X0, "*", "Region") 
{
    this->_marked    = false;
    this->_direction = RiverDirection::RIVER_NONE;
    
    this->settlement_name = "*";

    this->height      = 0.0f;
    this->latitude    = 0.0f;
    this->moisture    = 0.0f;
    this->temperature = 0.0f;
    this->visited     = false;
    
    this->owner = nullptr;
    this->unit  = nullptr;
    this->population.resize(0);

    this->map.resize(0);
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

bool Region::isPositionValid(const Building& building, sf::Vector2i grid_position) const {
    int index = world_settings.calculateRegionIndex(grid_position.x, grid_position.y);
    const auto building_size             = building.getBuildingArea();

    if(!world_settings.inRegionBounds(grid_position))
        return false;
    
    if(!world_settings.inRegionBounds(grid_position + sf::Vector2i(building_size.x - 1, building_size.y - 1)))
        return false;

    for(int y = 0; y < building_size.y; y++) {
        for(int x = 0; x < building_size.x; x++) {
            const int i = world_settings.calculateRegionIndex(grid_position.x, grid_position.y) + world_settings.calculateRegionIndex(x, y);

            if(!world_settings.inRegionBounds(i))
                return false;

            if(this->map.at(i).getElevation() != this->map.at(index).getElevation())
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

bool Region::isSpotOccupied(int index) const {
    if(!this->map.at(index).tiletype.is_terrain())
        return true;
    
    if(this->isTree(index))
        return true;
    
    if(this->getBuildingAt(index) != nullptr)
        return true;
    
    return false;
}

bool Region::isSpotOccupied(sf::Vector2i grid_position) const {
    auto index = world_settings.calculateRegionIndex(grid_position.x, grid_position.y);
    return this->isSpotOccupied(index);
}

void Region::placeBuilding(Building building, sf::Vector2f texture_size, sf::Vector2i grid_position) {
    int index = world_settings.calculateRegionIndex(grid_position.x, grid_position.y);
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

    else if(building == BUILDING_WELL)
        sp_building = std::shared_ptr <Building> (new Well());

    else if(building == BUILDING_WATER_COLLECTOR)
        sp_building = std::shared_ptr <Building> (new WaterCollector());

    else if(building == BUILDING_TOOLMAKER)
        sp_building = std::shared_ptr <Building> (new Toolmaker());

    else
        std::cout << "[Region][Place Building]: Undefined building type: " << building.getName() << "\n";

    for(int y = 0; y < building.getBuildingArea().y; y++) {
        for(int x = 0; x < building.getBuildingArea().x; x++) {
            const int i = index + world_settings.calculateRegionIndex(x, y);

            this->buildings[i] = std::shared_ptr <Building> (new Building());
            
            // The "empty" buildings are named to help recognise multi-tile structures.
            this->buildings[i].get()->setBuildingName(sp_building.get()->getBuildingName());
        }
    }
    
    sp_building.get()->assignAllProperties(building);
    this->buildings[index] = sp_building;
}

bool Region::placeBuildingCheck(Building building, sf::Vector2f texture_size, sf::Vector2i grid_position) {
    if(this->isPositionValid(building, grid_position) && this->isBuildingAffordable(building)) {
        this->placeBuilding(building, texture_size, grid_position);
        this->removeBuildingCost(building);
        return true;
    }

    return false;
}

void Region::removeBuilding(int index) {
    if(this->buildings.count(index)) {
        auto building   = this->buildings.at(index).get();
        
        if(!building->isRemovable())
            return;
        
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
                if(!this->isSameBuilding(*tile_building, building_index, index) && *tile_building == building)
                    buildings_in_proximity++;
            }
        }
    } 

    return buildings_in_proximity;
}

bool Region::isSameBuilding(const Building& building, int building_index, int index) const {
    bool same_building = false;

    if(building.getBuildingArea().x == 1)
        return building_index == index;

    auto building_size = building.getBuildingArea();

    for(int y = -building_size.y; y <= building_size.y; y++) {
        for(int x = -building_size.x; x <= building_size.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            
            if(building_index == index)
                return true;
        }
    }

    return false;
}

bool Region::isPath(int index) const {
    auto* building = this->getBuildingAt(index);

    if(building) {
        const auto& texture_name = building->getTextureName();
        return startsWith(texture_name, "path");
    }

    return false;
}

// Returns the index of a closest free (not occupied) tile relative to the centre of the map.
// If no free spot is found, returns -1.
int Region::findNotOccupiedTile(std::vector <int> buffer) const {
    sf::Vector2i middle = world_settings.getWorldWidth() % 2 == 0
        ? sf::Vector2i(world_settings.getRegionWidth() / 2, world_settings.getRegionWidth() / 2)
        : sf::Vector2i(world_settings.getRegionWidth() / 2 + 1, world_settings.getRegionWidth() / 2 + 1);
    
    const int middle_index = world_settings.calculateRegionIndex(middle.x, middle.y); 

    int size = 1;
    while(true) {
        for(int y = -size; y <= size; y++) {
            for(int x = -size; x <= size; x++) {
                sf::Vector2i normalised_grid;
                normalised_grid.x = (x <= 0)
                    ? x + world_settings.getRegionWidth() / 2
                    : x + world_settings.getRegionWidth() / 2 - 1;

                normalised_grid.y = (y <= 0)
                    ? y + world_settings.getRegionWidth() / 2
                    : y + world_settings.getRegionWidth() / 2 - 1;

                const int index     = middle_index + world_settings.calculateRegionIndex(x, y);
                const bool occupied = this->isSpotOccupied(index);
                
                if((occupied || std::find(buffer.begin(), buffer.end(), index) != buffer.end()) && y == size - 1 && x == size - 1) {
                    size++;
                    y = -size;
                }

                // If the index is not marked as unacceptable, then it is a free spot.
                else if(!occupied && std::find(buffer.begin(), buffer.end(), index) == buffer.end())
                    return index;
            }
        }
    }

    return -1;
}

int Region::getPopulation() const {
    return this->population.size();
}

bool Region::isPassableAStar(int index) const {
    if(this->map[index].tiletype.is_ocean())
        return false;
    
    if(this->getBuildingAt(index))
        return false;
    return true;
}