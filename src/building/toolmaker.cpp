#include "toolmaker.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

Toolmaker::Toolmaker()
    : Building(BUILDINGSIZE128X128, "building_toolmaker", "Toolmaker", "icon_default", 12, VECTOR2X2, VECTOR1X1)
{
    this->loadResourceCost();
}

Toolmaker::~Toolmaker() {

}

void Toolmaker::update(GameObject* object, int building_index) {
    auto region          = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    Resource tools = RESOURCE_TOOLS;

    if(region->resources.count(RESOURCE_WOOD.getName()) && region->resources.count(RESOURCE_FLINT.getName())) {
        Resource wood = RESOURCE_WOOD;
        wood.setQuantity(-1);

        Resource flint = RESOURCE_FLINT;
        flint.setQuantity(-1);

        tools.incrementQuantity();

        region->addResource(wood);
        region->addResource(flint);
        region->addResource(tools);
    }
}