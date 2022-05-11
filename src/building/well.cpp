#include "well.hpp"
#include "region.hpp"

using namespace iso;

Well::Well() 
    : Building(BUILDINGSIZE64X64, "building_well", "Well", "icon_default", 10, VECTOR1X1, VECTOR1X1) 
{}

Well::~Well() {

}

void Well::update(GameObject* object, int building_index) {
    auto region = static_cast<Region*>(object);
    
    Resource water = RESOURCE_WATER;
    water.setQuantity(1);

    region->addResource(water);
}