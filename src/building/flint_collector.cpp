#include "flint_collector.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

FlintCollector::FlintCollector()
    : Building(BUILDINGSIZE128X128, "building_flint_collector", "Flint Collector", "icon_default", 9, VECTOR2X2, VECTOR3X3, ResourceCollection(0, 0, 0))
{}

FlintCollector::~FlintCollector() {

}

void FlintCollector::update(GameObject* object, int building_index) {
    // Flint collector building.
    // A flint collector's efficiency depends on the number of flint tiles surrounding it.

    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    int flint_tiles = 0;
    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);

            if(region->map[index].object_texture_name == "tile_resource_flint")
                flint_tiles++;
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        flint_tiles /= number_of_buildings;

    region->addResource(ResourceType::RESOURCE_FLINT, flint_tiles);
}