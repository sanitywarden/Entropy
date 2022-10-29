Resource = {
    name = "Clay",
    description = "Description",
    texture = "tile_resource_clay",
    texture_size = Vector2i(64, 32),
    icon_texture = "icon_item_clay",
    icon_size = Vector2i(48, 48),
    type = "Base Material",
    minimum_occurence = 0,
    maximum_occurence = 1,
    generation_chance = 0.5,
    patch_size = 5,
    region = {
        "WATER"
    },
    tile = {
        "TERRAIN", "UNOCCUPIED", "COAST",
    }
}