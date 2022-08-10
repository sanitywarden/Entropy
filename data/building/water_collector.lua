Building = {
    name = "Water Collector",
    description = "Water Collector produces drinkable water",
    size = {
        x = 1,
        y = 1
    },
    texture = "building_water_collector",
    texture_size = {
        x = 64,
        y = 64
    },
    icon_texture = "icon_building_water_collector",
    icon_size = {
        x = 48,
        y = 48
    },
    removable = true,
    cost = {
        Wood = 10,
        Flint = 4,
        Stone = 2
    },
    refund = {
        Wood = 4,
        Flint = 2
    },
    produces = {
        name = "Water",
        amount = 1,
        requires = {}
    }
}