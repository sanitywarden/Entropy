#include "clay.hpp"
#include "region.hpp"
#include "globalutilities.hpp"
#include "generationSettings.hpp"
#include "resource_definitions.hpp"

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

    if(region->map[index].tiletype.is_water())
        return false;

    if(!startsWith(region->map[index].object_texture_name, "tile_grass"))    
        return false;

    auto range = world_settings.getRegionResourceRadius(RESOURCE_CLAY);
    for(int y = -range; y <= range; y++) {
        for(int x = -range; x <= range; x++) {
            auto i = index + world_settings.calculateRegionIndex(x, y);
            
            if(!world_settings.inRegionBounds(i))
                continue;

            const auto& tile = region->map[i];
            if(tile.tiletype.is_water())
                return true;
        }
    }

    return false;
}

void Clay::placeResource(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);
    region->map[index].object_texture_name = "tile_resource_clay";
}