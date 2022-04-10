#include "woodcutter.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

Woodcutter::Woodcutter() 
    : Building(VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_woodcutter", "Woodcutter", 6, VECTOR1X1, VECTOR3X3, ResourceCollection(0, 0, 0)) 
{}

Woodcutter::~Woodcutter() {

}

void Woodcutter::update(GameObject* object, int building_index) {
    // Woodcutter building.
    // A woodcutter's efficiency depends on the number of trees surrounding it.

    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    int trees = 0;
    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            
            if(region->isTree(index))
                trees++;
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        trees /= number_of_buildings;

    region->addResource(ResourceType::RESOURCE_WOOD, trees);
}