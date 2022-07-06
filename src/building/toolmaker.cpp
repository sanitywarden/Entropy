#include "toolmaker.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

Toolmaker::Toolmaker()
    : Building(BUILDINGSIZE128X128, "building_toolmaker", "Toolmaker", "icon_building_toolmaker", 12, VECTOR2X2, VECTOR1X1)
{
    this->loadResourceCost();
}

Toolmaker::~Toolmaker() {

}

void Toolmaker::update(GameObject* object, int building_index) {
    auto region = static_cast<Region*>(object);
    
    if(region->getItemQuantity(ITEM_WOOD) && region->getItemQuantity(ITEM_FLINT)) {
        StorageItem tools = ITEM_TOOLS;

        StorageItem wood = ITEM_WOOD;
        wood.quantity = -1;

        StorageItem flint = ITEM_FLINT;
        flint.quantity = -1;

        tools.quantity = 1;

        region->addItem(wood);
        region->addItem(flint);
        region->addItem(tools);
    }
}