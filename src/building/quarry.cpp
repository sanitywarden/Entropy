#include "quarry.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

Quarry::Quarry() 
    : Building(VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_quarry", "Quarry", 5, VECTOR1X1, VECTOR3X3, ResourceCollection(0, 0, 0))
{}

Quarry::~Quarry() {

}

void Quarry::update(GameObject* object, int building_index) {
    // Quarry building.
    // A quarry's efficiency depends on the number of stone tiles surrounding it.
    
    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    int stone_tiles = 0;
    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);

            if(region->map[index].object_texture_name == "tile_resource_stone")
                stone_tiles++;
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        stone_tiles /= number_of_buildings;

    region->addResource(ResourceType::RESOURCE_STONE, stone_tiles);
}