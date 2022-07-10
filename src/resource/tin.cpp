#include "tin.hpp"
#include "region.hpp"
#include "globalutilities.hpp"

using namespace iso;

Tin::Tin()
    : Resource("Tin", ResourceType::TYPE_RAW_MATERIAL, "icon_item_tin", 0, 1)
{}

Tin::~Tin()
{}

bool Tin::isGenerationSpotValid(GameObject* object, int index) const {
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

void Tin::placeResource(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);
    region->map[index].object_texture_name = "tile_resource_tin";    
}