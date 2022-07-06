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

    StorageItem grain = ITEM_GRAIN;

    // TODO: Rewrite this function to make it collect all types of collectable plants. 
   
    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);

            auto texture_name = region->map[index].getTextureName();
            if(startsWith(texture_name, "tile_grass"))
                grain.quantity++;
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        grain.quantity /= number_of_buildings;

    region->addItem(grain);
}

bool Farmhouse::isBuildingResourceTile(GameObject* object, int index) const { 
    auto region = static_cast<Region*>(object);
    return startsWith(region->map[index].object_texture_name, "tile_grass");
}