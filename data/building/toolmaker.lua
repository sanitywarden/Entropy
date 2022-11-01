Building = {
    name = "Toolmaker",
    description = "Toolmaker produces basic tools",
    size = Vector2i(2, 2),
    texture = "building_toolmaker",
    texture_size = Vector2i(128, 128),
    icon_texture = "icon_building_toolmaker",
    icon_size = Vector2i(48, 48),
    removable = true,
    cost = {
        Wood = 15,
    },
    refund = {
        Wood = 5
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
    },
}