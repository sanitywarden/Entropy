#include "flint_collector.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

FlintCollector::FlintCollector()
    : Building(BUILDINGSIZE128X128, "building_flint_collector", "Flint Collector", "icon_building_flint", 9, VECTOR2X2, VECTOR3X3)
{
    this->loadResourceCost();
}

FlintCollector::~FlintCollector() {

}

void FlintCollector::update(GameObject* object, int building_index) {
    // Flint collector building.
    // A flint collector's efficiency depends on the number of flint tiles surrounding it.

    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    StorageItem flint = ITEM_FLINT;

    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);

            if(region->map[index].object_texture_name == "tile_resource_flint")
                flint.quantity++;
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        flint.quantity /= number_of_buildings;

    region->addItem(flint);
}

bool FlintCollector::isBuildingResourceTile(GameObject* object, int index) const {
    auto region = static_cast<Region*>(object);
    return region->map[index].object_texture_name == "tile_resource_flint";
}