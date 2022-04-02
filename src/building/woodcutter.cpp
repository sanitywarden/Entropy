#include "woodcutter.hpp"
#include "region.hpp"
#include <iostream>

using namespace iso;

Woodcutter::Woodcutter() 
    : Building(VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_woodcutter", "Woodcutter", 6, VECTOR1X1, ResourceCollection(0, 0, 0)) 
{}

Woodcutter::~Woodcutter() {

}

void Woodcutter::update(GameObject* object, int building_index) {
    // Woodcutter building.
    // A woodcutter's efficiency depends on the number of trees surrounding it.

    auto region = static_cast<Region*>(object);

    const int tree_scan_size = 5;
    int trees = 0;
    for(int y = -tree_scan_size; y <= tree_scan_size; y++) {
        for(int x = -tree_scan_size; x <= tree_scan_size; x++) {
            const int index = building_index + y * this->generation_settings.region_size + x;
            
            if(region->trees.count(index))
                trees++;
        }
    }

    region->addResource(ResourceType::RESOURCE_WOOD, trees);
}