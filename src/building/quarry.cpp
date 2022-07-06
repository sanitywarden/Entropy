#include "quarry.hpp"
#include "region.hpp"
#include "generationSettings.hpp"
#include <iostream>

using namespace iso;

Quarry::Quarry() 
    : Building(BUILDINGSIZE128X128, "building_quarry", "Quarry", "icon_building_quarry", 5, VECTOR2X2, VECTOR3X3)
{
    this->loadResourceCost();
}

Quarry::~Quarry() {

}

void Quarry::update(GameObject* object, int building_index) {
    // Quarry building.
    // A quarry's efficiency depends on the number of stone tiles surrounding it.
    
    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    StorageItem stone = ITEM_STONE;

    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);

            if(region->map[index].object_texture_name == "tile_resource_stone")
                stone.quantity++;
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        stone.quantity /= number_of_buildings;

    region->addItem(stone);
}

bool Quarry::isBuildingResourceTile(GameObject* object, int index) const {
    auto region = static_cast<Region*>(object);
    return region->map[index].object_texture_name == "tile_resource_stone";
}
