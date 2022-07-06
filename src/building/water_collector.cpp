#include "water_collector.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

WaterCollector::WaterCollector() 
    : Building(BUILDINGSIZE64X64, "building_water_collector", "Water Collector", "icon_building_water_collector", 11, VECTOR1X1, VECTOR2X2) 
{
    this->loadResourceCost();
}

WaterCollector::~WaterCollector() {

}

void WaterCollector::update(GameObject* object, int building_index) {
    // Water collector building.
    // A water collector's efficiency depends on the number of water surrounding it.

    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    StorageItem water = ITEM_WATER;

    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            
            if(!region->map[index].tiletype.is_terrain())
                water.quantity++;
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        water.quantity /= number_of_buildings;

    region->addItem(water);
}

bool WaterCollector::isBuildingResourceTile(GameObject* object, int index) const {
    auto region = static_cast<Region*>(object);
    return region->map[index].tiletype.is_water();    
}
