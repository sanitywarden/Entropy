#include "quarry.hpp"
#include "region.hpp"
#include "generationSettings.hpp"
#include <iostream>

using namespace iso;

Quarry::Quarry() 
    : Building(BUILDINGSIZE128X128, "building_quarry", "Quarry", "icon_building_quarry", 5, VECTOR2X2, VECTOR3X3)
{
    this->loadResourceCost();
}

Quarry::~Quarry() {

}

void Quarry::update(GameObject* object, int building_index) {
    // Quarry building.
    // A quarry's efficiency depends on the number of stone tiles surrounding it.
    
    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    // There might be more than 1 item collected.
    std::vector <StorageItem> items;

    StorageItem stone = ITEM_STONE;

    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);
            const auto& tile = region->map[index];

            if(tile.hasResource()) {
                if(tile.getResource()->resource_type == ResourceType::TYPE_RAW_MATERIAL || *tile.getResource() == RESOURCE_STONE) {
                    for(auto it = items.begin(); it != items.end(); ++it) {
                        auto item = *it;
                        if(item.item_name == tile.getResource()->resource_name)
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

bool Quarry::isBuildingResourceTile(GameObject* object, int index) const {
    auto region = static_cast<Region*>(object);
    const auto& tile = region->map[index];

    return tile.hasResource() 
        ? (tile.getResource()->resource_type == ResourceType::TYPE_RAW_MATERIAL || *tile.getResource() == RESOURCE_STONE)
        : false;
}
