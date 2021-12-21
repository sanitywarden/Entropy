#include "buildingManager.hpp"

#include "regionmap.hpp"

using namespace iso;

BuildingManager::BuildingManager() {
    this->manager = nullptr;
}

BuildingManager::BuildingManager(SimulationManager* manager) {
    this->manager = manager;
}

BuildingManager::~BuildingManager() {
    
}

bool BuildingManager::isPositionValid(int index, Building building) const {
    Regionmap* regionmap = static_cast<Regionmap*>(this->manager->gamestate.getGamestate());
    Region*    region    = regionmap->getCurrentRegion();

    const auto building_size    = building.building_size;
    const auto foundation_tile_elevation = region->map[index].elevation; 

    for(int x = 0; x < building_size.x; x++) {
        for(int y = 0; y < building_size.y; y++) {
            const int i = (y * this->manager->region_settings.size.x) + index + x;

            if(region->map[i].elevation != foundation_tile_elevation)
                return false;

            if(!region->map[i].tiletype.is_terrain())
                return false;

            if(region->trees[i].exists())
                return false;
        }
    }

    return true;
}

void BuildingManager::placeBuilding(int index, Building building) const {
    Regionmap* regionmap = static_cast<Regionmap*>(this->manager->gamestate.getGamestate());
    Region*    region    = regionmap->getCurrentRegion();

    region->buildings.insert({ index, building });
}

bool BuildingManager::placeBuildingCheck(int index, Building building) const {
    if(this->isPositionValid(index, building)) {
        this->placeBuilding(index, building);
        return true;
    }

    return false;
}

void BuildingManager::removeBuilding(int index) const {
    Regionmap* regionmap = static_cast<Regionmap*>(this->manager->gamestate.getGamestate());
    Region*    region    = regionmap->getCurrentRegion();

    if(region->buildings.erase(index));
}
