#include "clay.hpp"
#include "region.hpp"
#include "globalutilities.hpp"
#include "generationSettings.hpp"
#include "resource_definitions.hpp"
#include <iostream>

using namespace iso;

Clay::Clay()
    : Resource("Clay", ResourceType::TYPE_BASE_MATERIAL, "icon_item_clay", 0, 2)
{}

Clay::~Clay()
{}

bool Clay::isGenerationSpotValid(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);

    if(region->getBuildingAt(index))
        return false;

    if(region->isTree(index))
        return false;

    if(region->getBuildingAt(index))
        return false;

    if(!startsWith(region->map[index].object_texture_name, "tile_grass"))    
        return false;

    if(region->map[index].tiletype.is_water())
        return false;

    auto index_left   = index - 1;
    auto index_right  = index + 1;
    auto index_top    = index - world_settings.getRegionWidth();
    auto index_bottom = index + world_settings.getRegionWidth();

    bool left   = false;
    bool right  = false;
    bool top    = false;
    bool bottom = false;

    if(world_settings.inRegionBounds(index_left))
        left = region->map[index_left].tiletype.is_water() || region->map[index_left].getTextureName() == "tile_resource_clay";
    
    if(world_settings.inRegionBounds(index_right))
        right = region->map[index_right].tiletype.is_water() || region->map[index_right].getTextureName() == "tile_resource_clay";

    if(world_settings.inRegionBounds(index_top))
        top = region->map[index_top].tiletype.is_water() || region->map[index_top].getTextureName() == "tile_resource_clay";
    
    if(world_settings.inRegionBounds(index_bottom))
        bottom = region->map[index_bottom].tiletype.is_water() || region->map[index_bottom].getTextureName() == "tile_resource_clay";
    
    return (left || right || top || bottom);
}

void Clay::placeResource(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);
    region->map[index].object_texture_name = "tile_resource_clay";
}

bool Clay::canBeGenerated(GameObject* object) const {
    auto* region = static_cast<Region*>(object);
    return (region->regiontype.is_coast() || region->regiontype.is_river());
}