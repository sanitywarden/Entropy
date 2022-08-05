Building = {
    name = "Toolmaker",
    description = "Toolmaker produces basic tools",
    size = {
        x = 2,
        y = 2
    },
    texture = "building_toolmaker",
    texture_size = {
        x = 128,
        y = 128
    },
    icon_texture = "icon_building_toolmaker",
    icon_size = {
        x = 48,
        y = 48
    },
    removable = true,
    cost = {
        Wood = 15,
    },
    refund = {
        Wood = 5
    },
    scan_area = {
        x = 3,
        y = 3
    },
    produces = {
        {
            name = "Tools",
            amount = 1,
            requires = {
                Wood = 1,
                Stone = 1
            }
        }
    }
}