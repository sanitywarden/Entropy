Building = {
    name = "Well",
    description = "Well collects drinkable groundwater from the ground",
    size = {
        x = 1,
        y = 1
    },
    texture = "building_well",
    texture_size = {
        x = 64,
        y = 64
    },
    icon_texture = "icon_building_well",
    icon_size = {
        x = 48,
        y = 48
    },
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