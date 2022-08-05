Building = {
    name = "Woodcutter",
    description = "Woodcutter cuts surrounding trees for wood",
    size = {
        x = 2,
        y = 2
    },
    texture = "building_woodcutter",
    texture_size = {
        x = 128,
        y = 128
    },
    icon_texture = "icon_building_woodcutter",
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
            name = "Tree",
            harvest = {
                Wood = 2
            }
        }
    }
}