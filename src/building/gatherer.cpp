#include "gatherer.hpp"
#include "globalutilities.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

Farmhouse::Farmhouse() 
    : Building(BUILDINGSIZE128X128, "building_gatherer", "Gatherer", "icon_building_gatherer", 4, VECTOR2X2, VECTOR3X3) 
{
    this->loadResourceCost();
}

Farmhouse::~Farmhouse() {

}

void Farmhouse::update(GameObject* object, int building_index) {
    // Farm building.
    // A farm's efficiency depends on the number of grass tiles surrounding it.
    
    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    // There might be more than 1 item collected.
    std::vector <StorageItem> items;

    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            const auto& tile = region->map[index];
            
            if(region->tileHasResource(index)) {
                if(region->getTileResource(index)->getResourceType() == ResourceType::TYPE_PLANT_RESOURCE || region->getTileResource(index)->getResourceType() == ResourceType::TYPE_PLANT_FOOD) {
                    for(auto it = items.begin(); it != items.end(); ++it) {
                        auto item = *it;
                        if(item.item_name == region->getTileResource(index)->getResourceName())
                            item.quantity++;
                    }
                }     
            }
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings) {
        for(auto& item : items) {
            item.quantity /= number_of_buildings;
            region->addItem(item);
        }
    }
}

bool Farmhouse::isBuildingResourceTile(GameObject* object, int index) const { 
    auto region = static_cast<Region*>(object);
    return region->tileHasResource(index) 
        ? region->getTileResource(index)->getResourceType() == ResourceType::TYPE_PLANT_RESOURCE || region->getTileResource(index)->getResourceType() == ResourceType::TYPE_PLANT_FOOD
        : false;
}