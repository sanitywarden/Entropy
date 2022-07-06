#include "well.hpp"
#include "region.hpp"

using namespace iso;

Well::Well() 
    : Building(BUILDINGSIZE64X64, "building_well", "Well", "icon_building_well", 10, VECTOR1X1, VECTOR1X1) 
{
    this->loadResourceCost();
}

Well::~Well() {

}

void Well::update(GameObject* object, int building_index) {
    auto region = static_cast<Region*>(object);
    
    StorageItem water = ITEM_WATER;
    water.quantity = 8;

    region->addItem(water);
}