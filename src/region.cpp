#include "region.hpp"
#include "globalutilities.hpp"
#include "worldData.hpp"

#include <iostream>
#include <string>

using namespace iso;

Region::Region() 
    : GameObject(core::Vector3f(0, 0, 0), core::Vector3f(0, 0, 0), core::Vector2i(0, 0), "*", "Region") 
{
    this->_marked    = false;
    this->_direction = RiverDirection::RIVER_NONE;
    
    this->settlement_name = "*";

    this->height      = 0.0f;
    this->latitude    = 0.0f;
    this->moisture    = 0.0f;
    this->temperature = 0.0f;
    this->visited     = false;
    
    this->map.resize(0);
    this->stockpile.resize(0);
}

Region::~Region() 
{}

RiverDirection Region::riverDirection() {
    return this->_direction;
}

bool Region::isOwned() const {
    return false;
}

int Region::getOwnerId() const {
    return -1;
}

void Region::stockpileAdd(StorageItem item) {
    if(this->itemExists(item)) {
        for(auto it = this->stockpile.begin(); it != this->stockpile.end(); ++it) {
            auto storage_item = *it;

            if(storage_item.getItemName() == item.getItemName()) {
                storage_item.setAmount(storage_item.getAmount() + item.getAmount());
                return;
            }
        }
    }
    
    this->stockpile.push_back(item);
}

void Region::stockpileRemove(StorageItem item) {
    if(this->itemExists(item)) {
        for(auto it = this->stockpile.begin(); it != this->stockpile.end(); ++it) {
            auto storage_item = *it;

            if(storage_item.getItemName() == item.getItemName())
                this->stockpile.erase(it);
        }
    }
}

int Region::itemQuantity(const StorageItem& item) const {
    if(this->itemExists(item)) {
        for(auto it = this->stockpile.begin(); it != this->stockpile.end(); ++it) {
            auto storage_item = *it;
            
            if(storage_item.getItemName() == item.getItemName())
                return storage_item.getAmount();
        }
    }

    return 0;
}

bool Region::itemExists(const StorageItem& item) const {
    for(auto it = this->stockpile.begin(); it != this->stockpile.end(); ++it) {
        auto storage_item = *it;
        
        if(storage_item.getItemName() == item.getItemName())
            return true;
    }

    return false;
}

bool Region::isPassableWorldmap() const {
    return false;
}

bool Region::isPassableRegionmap(int index) const {
    return false;
} 

bool Region::isPassableRegionmap(core::Vector2i grid) const {
    return false;
}

// int Region::getFoodQuantity() const {
//     int quantity = 0;
//     for(const auto& item : ITEM_LOOKUP_TABLE) {
//         if(this->checkItemExists(item) && item.item_type == ItemType::TYPE_FOOD) {
//             quantity += this->getItemQuantity(item);
//         }
//     }  

//     return quantity;
// }

// int Region::getDrinkableLiquidQuantity() const {
//     int quantity = 0;
//     for(const auto& item : ITEM_LOOKUP_TABLE) {
//         if(this->checkItemExists(item) && item.item_type == ItemType::TYPE_DRINKABLE_LIQUID) {
//             quantity += this->getItemQuantity(item);
//         }
//     }  

//     return quantity;
// }

// bool Region::isBuildingAffordable(const Building& building) const {
//  return (this->resources >= building.getBuildingCost());
// }

/* Remove the resources needed to construct provided building from region's resource pool. */
// void Region::removeBuildingCost(const Building& building) {
    // this->resources -= building.getBuildingCost();
// }

bool Region::isBuildingPositionValid(const Building& building, core::Vector2i grid) const {
    auto index = calculateRegionIndex(grid);
    auto building_size = building.getBuildingArea();

    if(!inRegionBounds(grid))
        return false;
    
    if(!inRegionBounds(grid + core::Vector2i(building_size.x - 1, building_size.y - 1)))
        return false;

    for(int y = 0; y < building_size.y; y++) {
        for(int x = 0; x < building_size.x; x++) {
            auto grid_copy = grid + core::Vector2i(x, y);
            auto loop_index = calculateRegionIndex(grid_copy);

            if(!inRegionBounds(loop_index))
                return false;

            if(this->map.at(loop_index).getElevation() != this->map.at(index).getElevation())
                return false;

            if(!this->map.at(loop_index).tiletype.is_terrain())
                return false;

            if(this->treeExistsAt(grid_copy))
                return false;

            if(this->buildingExistsAtGrid(grid_copy))
                return false;
        }
    }

    return true;
}

void Region::removeBuildingCost(const Building& building) {
    // TODO: Write implementation.
}

bool Region::isSpotOccupied(int index) const {
    if(!inRegionBounds(index))
        return false;

    if(!this->map.at(index).tiletype.is_terrain())
        return true;
    
    if(this->treeExistsAt(index))
        return true;
    
    if(this->buildingExistsAtIndex(index))
        return true;
    return false;
}

bool Region::isSpotOccupied(core::Vector2i grid_position) const {
    auto index = calculateRegionIndex(grid_position);
    return this->isSpotOccupied(index);
}

// void Region::placeBuilding(Building building, sf::Vector2f texture_size, core::Vector2i grid_position) {

// }

// bool Region::placeBuildingCheck(Building building, sf::Vector2f texture_size, core::Vector2i grid_position) {
//     if(this->isPositionValid(building, grid_position) && this->isBuildingAffordable(building)) {
//         this->placeBuilding(building, texture_size, grid_position);
        
//         if(world_data.buildingCostEnabled())
//             this->removeBuildingCost(building);
        
//         return true;
//     }

//     return false;
// }

// void Region::removeBuilding(int index) {
//     if(this->buildings.count(index)) {
//         auto building = this->buildings.at(index).get();
        
//         if(!building->isRemovable())
//             return;
        
//         if(world_data.buildingCostEnabled()) {
//             auto refund = building->getBuildingRefund();
//             for(auto resource : refund) {
//                 this->addItem(resource);
//             }
//         }
        
//         for(int y = 0; y < building->getBuildingArea().y; y++) {
//             for(int x = 0; x < building->getBuildingArea().x; x++) {
//                 const int i = index + world_data.calculateRegionIndex(x, y);
//                 this->buildings.erase(i);
//             }
//         }
//     }
// }   

// Returns the number of buildings buildings in proximity.
// Proximity is understood as the building's production area.
// int Region::isBuildingInProximity(const Building& building, int building_index) const {
//     const auto search_area = building.getBuildingScanArea();
//     int buildings_in_proximity = 0;

//     for(int y = -search_area.y; y <= search_area.y; y++) {
//         for(int x = -search_area.x; x <= search_area.x; x++) {
//             const int index = building_index + world_data.calculateRegionIndex(x, y);

//             // Building or a nullptr.
//             auto tile_building = this->getBuildingAt(index);
//             if(tile_building) {
//                 if(!this->isSameBuilding(*tile_building, building_index, index) && *tile_building == building)
//                     buildings_in_proximity++;
//             }
//         }
//     } 

//     return buildings_in_proximity;
// }

// bool Region::isSameBuilding(const Building& building, int building_index, int index) const {
//     bool same_building = false;

//     if(building.getBuildingArea().x == 1)
//         return building_index == index;

//     auto building_size = building.getBuildingArea();

//     for(int y = -building_size.y; y <= building_size.y; y++) {
//         for(int x = -building_size.x; x <= building_size.x; x++) {
//             const int index = building_index + world_data.calculateRegionIndex(x, y);
            
//             if(building_index == index)
//                 return true;
//         }
//     }

//     return false;
// }

// Returns the index of a closest free (not occupied) tile relative to the centre of the map.
// If no free spot is found, returns -1.
// int Region::findNotOccupiedTile(std::vector <int> buffer) const {
//     core::Vector2i middle = world_data.getWorldWidth() % 2 == 0
//         ? core::Vector2i(world_data.getRegionWidth() / 2, world_data.getRegionWidth() / 2)
//         : core::Vector2i(world_data.getRegionWidth() / 2 + 1, world_data.getRegionWidth() / 2 + 1);
    
//     const int middle_index = world_data.calculateRegionIndex(middle.x, middle.y); 

//     int size = 1;
//     while(true) {
//         for(int y = -size; y <= size; y++) {
//             for(int x = -size; x <= size; x++) {
//                 core::Vector2i normalised_grid;
//                 normalised_grid.x = (x <= 0)
//                     ? x + world_data.getRegionWidth() / 2
//                     : x + world_data.getRegionWidth() / 2 - 1;

//                 normalised_grid.y = (y <= 0)
//                     ? y + world_data.getRegionWidth() / 2
//                     : y + world_data.getRegionWidth() / 2 - 1;

//                 const int index     = middle_index + world_data.calculateRegionIndex(x, y);
//                 const bool occupied = this->isSpotOccupied(index);
                
//                 if((occupied || std::find(buffer.begin(), buffer.end(), index) != buffer.end()) && y == size - 1 && x == size - 1) {
//                     size++;
//                     y = -size;
//                 }

//                 // If the index is not marked as unacceptable, then it is a free spot.
//                 else if(!occupied && std::find(buffer.begin(), buffer.end(), index) == buffer.end())
//                     return index;
//             }
//         }
//     }

//     return -1;
// }

// bool Region::isPassableRegionmap(int index) const {
//     if(this->map[index].tiletype.is_ocean())
//         return false;
    
//     if(this->buildingExistsAt(index))
//         return false;
//     return true;
// }

bool Region::treeExistsAt(int index) const {
    return this->trees.count(index);
}

bool Region::treeExistsAt(core::Vector2i grid) const {
    auto index = calculateRegionIndex(grid);
    return this->treeExistsAt(index);
}

bool Region::resourceExistsAt(int index) const {
    return this->resources.count(index);
}

bool Region::resourceExistsAt(core::Vector2i grid) const {
    auto index = calculateRegionIndex(grid);
    return this->resourceExistsAt(index);
}

const Resource& Region::getResourceAt(int index) const {
    return this->resources.at(index);
}

const Resource& Region::getResourceAt(core::Vector2i grid) const {
    auto index = calculateRegionIndex(grid);
    return this->getResourceAt(index);
}

bool Region::buildingExistsAtIndex(int index) const {
    return this->buildings.count(index);
}

bool Region::buildingExistsAtGrid(core::Vector2i grid) const {
    auto index = calculateRegionIndex(grid);
    return this->buildingExistsAtIndex(index);
}

const Building& Region::getBuildingAtIndex(int index) const {
    return this->buildings.at(index);
}

const Building& Region::getBuildingAtGrid(core::Vector2i grid) const {
    auto index = calculateRegionIndex(grid);
    return this->getBuildingAtIndex(index);
}

Tile& Region::getTileAtIndex(int index) {
    if(!inRegionBounds(index))
        printError("Region::getTileAtIndex()", "Requested tile is out of bounds");
    return this->map.at(index);
}

bool Region::isBiome(const Biome& biome) const {
    return this->biome == biome;
}