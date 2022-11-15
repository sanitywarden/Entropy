Resource = {
    name = "Tin",
    description = "Description",
    texture = "tile_resource_tin",
    texture_size = Vector2i(64, 32),
    icon_texture = "icon_item_tin",
    icon_size = Vector2i(48, 48),
    type = "Raw Material",
    minimum_occurence = 0,
    maximum_occurence = 1,
    generation_chance = 0.1, 
    patch_size = 3,
    isRegionValid = function(region_index)
        region = getRegion(region_index)
        return region:isTerrain()
    end,
    isTileValid = function(region_index, tile_index)
        region = getRegion(region_index)
        tile = region:getTileAt(tile_index)
        return tile:isTerrain() and not region:isTileOccupied(tile_index) 
    end
}