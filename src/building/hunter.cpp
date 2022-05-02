#include "hunter.hpp"
#include "region.hpp"
#include "generationSettings.hpp"
#include "building_definitions.hpp"

#include <iostream>

using namespace iso;

Hunter::Hunter()
    : Building(BUILDINGSIZE128X128, "building_hunter", "Hunter", "icon_default", 7, VECTOR2X2, VECTOR3X3, ResourceCollection(0, 0, 0))
{}

Hunter::~Hunter() {

}

void Hunter::update(GameObject* object, int building_index) {
    // Hunter building.
    // A hunter's efficiency depends on the presence of animals in the building production area. 

    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    bool animal_present = false;
    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            
            auto building = region->getBuildingAt(index);
            if(building) {
                if(*building == BUILDING_ANIMAL_SPOT) {
                    animal_present = true;
                    break;
                }
            }
        }
    }

    if(animal_present) {
        region->addResource(ResourceType::RESOURCE_FOOD, 10);
    }
}