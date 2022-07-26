#include "stone.hpp"
#include "region.hpp"
#include "globalutilities.hpp"

using namespace iso;

Stone::Stone()
    : Resource("Stone", ResourceType::TYPE_BASE_MATERIAL, "icon_item_stone", 0, 2)
{}

Stone::~Stone()
{}

bool Stone::isGenerationSpotValid(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);

    if(region->isTree(index))
        return false;

    if(region->getBuildingAt(index))
        return false;

    if(region->map[index].tiletype.is_water())
        return false;

    if(!startsWith(region->map[index].object_texture_name, "tile_grass"))    
        return false; 
    
    return true;
}

void Stone::placeResource(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);
    region->map[index].object_texture_name = "tile_resource_stone";
    region->resources[index] = getResourcePointer("Stone");
}