#include "water_collector.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

WaterCollector::WaterCollector() 
    : Building(BUILDINGSIZE64X64, "building_water_collector", "Water Collector", "icon_default", 11, VECTOR1X1, VECTOR2X2) 
{}

WaterCollector::~WaterCollector() {

}

void WaterCollector::update(GameObject* object, int building_index) {
    // Water collector building.
    // A water collector's efficiency depends on the number of water surrounding it.

    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    Resource water = RESOURCE_WATER;

    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            
            if(!region->map[index].tiletype.is_terrain())
                water.incrementQuantity();
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        water.setQuantity(water.getQuantity() / number_of_buildings);

    region->addResource(water);
}
