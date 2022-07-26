#include "flint.hpp"
#include "region.hpp"
#include "globalutilities.hpp"

using namespace iso;

Flint::Flint()
    : Resource("Flint", ResourceType::TYPE_BASE_MATERIAL, "icon_item_flint", 0, 3)
{}

Flint::~Flint()
{}

bool Flint::isGenerationSpotValid(GameObject* object, int index) const {
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

void Flint::placeResource(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);
    region->map[index].object_texture_name = "tile_resource_flint";
    region->resources[index] = getResourcePointer("Flint");
}