Resource = {
    name = "Flint",
    description = "Description",
    texture = "tile_resource_flint",
    texture_size = Vector2i(64, 32),
    icon_texture = "icon_item_flint",
    icon_size = Vector2i(48, 48),
    type = "Base Material",
    minimum_occurence = 1,
    maximum_occurence = 2,
    generation_chance = 0.5,
    patch_size = 5,
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
