Building = {
    name = "Hunter",
    description = "Hunter provides meat and leather from animals",
    size = {
        x = 2,
        y = 2
    },
    texture = "building_hunter",
    texture_size = {
        x = 128,
        y = 128
    },
    icon_texture = "icon_building_hunter",
    icon_size = {
        x = 48,
        y = 48
    },
    removable = true,
    cost = {
        Wood = 10,
    },
    refund = {
        Wood = 5
    },
    scan_area = {
        x = 3,
        y = 3
    },
    harvests = {
        {
            name = "Animal",
            harvest = {
                Meat = 4,
                Leather = 2
            }
        }
    }
}