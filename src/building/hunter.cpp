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

    Resource meat    = RESOURCE_MEAT;
    Resource leather = RESOURCE_LEATHER; 

    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            
            auto building = region->getBuildingAt(index);
            if(building) {
                if(building->getBuildingName() == "Animal Spot") {
                    meat.incrementQuantity();
                }
            }
        }
    }

    leather.setQuantity(meat.getQuantity() / 4);

    region->addResource(meat);
    region->addResource(leather);
}

bool Hunter::isBuildingResourceTile(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);
    return region->getBuildingAt(index)
        ? region->getBuildingAt(index)->getBuildingName() == "Animal Spot"
        : false;
}
