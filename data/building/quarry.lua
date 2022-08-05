Building = {
    name = "Quarry",
    description = "Quarry excavates resources from the ground",
    size = {
        x = 2,
        y = 2
    },
    texture = "building_quarry",
    texture_size = {
        x = 128,
        y = 128
    },
    icon_texture = "icon_building_quarry",
    icon_size = {
        x = 48,
        y = 48
    },
    removable = true,
    cost = {
        Wood = 10,
        Tools = 3
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
            name = "Stone",
            harvest = {
                Stone = 1
            }
        }
    }
}