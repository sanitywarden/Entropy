#include "farmhouse.hpp"
#include "globalutilities.hpp"
#include "region.hpp"
#include "generationSettings.hpp"

using namespace iso;

Farmhouse::Farmhouse() 
    : Building(BUILDINGSIZE64X64, "building_farmland", "Farmhouse", "icon_building_farmhouse", 4, VECTOR1X1, VECTOR3X3, ResourceCollection(0, 0, 0)) 
{}

Farmhouse::~Farmhouse() {

}

void Farmhouse::update(GameObject* object, int building_index) {
    // Farm building.
    // A farm's efficiency depends on the number of grass tiles surrounding it.
    
    auto region = static_cast<Region*>(object);
    auto production_area = this->getProductionArea();

    int grass_tiles = 0;
    for(int y = -production_area.y; y <= production_area.y; y++) {
        for(int x = -production_area.x; x <= production_area.x; x++) {
            const int index = building_index + world_settings.calculateRegionIndex(x, y);

            auto texture_name = region->map[index].getTextureName();
            if(startsWith(texture_name, "tile_grass"))
                grass_tiles++;
        }
    }

    auto number_of_buildings = region->isBuildingInProximity(*this, building_index);
    if(number_of_buildings)
        grass_tiles /= number_of_buildings;

    region->addResource(ResourceType::RESOURCE_FOOD, grass_tiles);
}