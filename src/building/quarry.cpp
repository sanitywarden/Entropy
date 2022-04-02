#include "quarry.hpp"
#include "region.hpp"

using namespace iso;

Quarry::Quarry() 
    : Building(VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_quarry", "Quarry", 5, VECTOR1X1, ResourceCollection(0, 0, 0))
{}

Quarry::~Quarry() {

}

void Quarry::update(GameObject* object, int building_index) {
    // Quarry building.
    // A quarry's efficiency depends on the number of stone tiles surrounding it.
    
    auto region = static_cast<Region*>(object);

    const int stone_scan_size = 3;
    int       stone_tiles = 0;

    for(int y = -stone_scan_size; y <= stone_scan_size; y++) {
        for(int x = -stone_scan_size; x <= stone_scan_size; x++) {
            const int index = building_index + y * this->generation_settings.region_size + x;

            if(region->map[index].object_texture_name == "tile_resource_stone")
                stone_tiles++;
        }
    }

    region->addResource(ResourceType::RESOURCE_STONE, stone_tiles);
}