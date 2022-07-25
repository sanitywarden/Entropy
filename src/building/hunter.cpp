#include "hunter.hpp"
#include "region.hpp"
#include "generationSettings.hpp"
#include "building_definitions.hpp"

#include <iostream>

using namespace iso;

Hunter::Hunter()
    : Building(BUILDINGSIZE128X128, "building_hunter", "Hunter", "icon_building_hunter", 7, VECTOR2X2, VECTOR3X3)
{
    this->loadResourceCost();
}

Hunter::~Hunter() {

}

void Hunter::update(GameObject* object, int building_index) {
    // Hunter building.
    // A hunter's efficiency depends on the presence of animals in the building production area. 

    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    StorageItem meat    = ITEM_MEAT;
    StorageItem leather = ITEM_LEATHER; 

    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            const auto& tile = region->map[index];

            if(tile.hasResource()) {
                if(tile.getResource()->resource_type == ResourceType::TYPE_ANIMAL_FOOD) {
                    meat.quantity    += 2;
                    leather.quantity += 1;
                }
            }
        }
    }

    region->addItem(meat);
    region->addItem(leather);
}

bool Hunter::isBuildingResourceTile(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);
    const auto& tile = region->map[index];

    return tile.hasResource()
        ? tile.getResource()->resource_type == ResourceType::TYPE_ANIMAL_FOOD
        : false;
}
