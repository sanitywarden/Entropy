Building = {
    name = "Woodcutter",
    description = "Woodcutter cuts surrounding trees for wood",
    size = Vector2i(2, 2),
    texture = "building_woodcutter",
    texture_size = Vector2i(128, 128),
    icon_texture = "icon_building_woodcutter",
    icon_size = Vector2i(48, 48),
    removable = true,
    cost = {
        Wood = 10,
    },
    refund = {
        Wood = 5
    },
    scan_area = Vector2i(3, 3),
    harvests = {
        {
            name = "Tree",
            harvest = {
                Wood = 2
            }
        }
    }
}