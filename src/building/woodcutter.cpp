#include "woodcutter.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

Woodcutter::Woodcutter() 
    : Building(BUILDINGSIZE128X128, "building_woodcutter", "Woodcutter", "icon_building_woodcutter", 6, VECTOR2X2, VECTOR3X3) 
{
    this->loadResourceCost();
}

Woodcutter::~Woodcutter() {

}

void Woodcutter::update(GameObject* object, int building_index) {
    // Woodcutter building.
    // A woodcutter's efficiency depends on the number of trees surrounding it.

    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    Resource wood = RESOURCE_WOOD;

    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            
            if(region->isTree(index))
                wood.incrementQuantity();
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        wood.setQuantity(wood.getQuantity() / number_of_buildings);

    region->addResource(wood);
}