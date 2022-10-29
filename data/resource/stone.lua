Resource = {
    name = "Stone",
    description = "Description",
    texture = "tile_resource_stone",
    texture_size = Vector2i(64, 32),
    icon_texture = "icon_item_stone",
    icon_size = Vector2i(48, 48),
    type = "Base Material",
    minimum_occurence = 0,
    maximum_occurence = 1,
    generation_chance = 0.4, 
    patch_size = 4,
    region = {
        "ALL",
    },
    tile = {
        "TERRAIN", "UNOCCUPIED",
    }
}