#include "copper.hpp"
#include "region.hpp"
#include "globalutilities.hpp"

using namespace iso;

Copper::Copper()
    : Resource("Copper", iso::ResourceType::TYPE_RAW_MATERIAL, "icon_item_copper", 0, 1)
{}

Copper::~Copper()
{}

bool Copper::isGenerationSpotValid(GameObject* object, int index) const {
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

void Copper::placeResource(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);

    std::string variation = rand() % 100 < 25 
        ? "tile_resource_copper_1"
        : "tile_resource_copper_2";

    region->map[index].object_texture_name = variation;
}