Building = {
    name = "Well",
    description = "Well collects drinkable groundwater from the ground",
    size = Vector2i(1, 1),
    texture = "building_well",
    texture_size = Vector2i(64, 64),
    icon_texture = "icon_building_well",
    icon_size = Vector2i(48, 48),
    removable = true,
    cost = {
        Stone = 6,
        Wood = 2
    },
    refund = {
        Stone = 2,
        Wood = 1
    },
    produces = {
        name = "Water",
        amount = 1,
        requires = {}
    }
}
