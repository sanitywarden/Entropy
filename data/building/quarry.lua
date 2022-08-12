Building = {
    name = "Quarry",
    description = "Quarry excavates resources from the ground",
    size = Vector2i(2, 2),
    texture = "building_quarry",
    texture_size = Vector2i(128, 128),
    icon_texture = "icon_building_quarry",
    icon_size = Vector2i(48, 48),
    removable = true,
    cost = {
        Wood = 10,
        Tools = 3
    },
    refund = {
        Wood = 5
    },
    scan_area = Vector2i(3, 3),
    harvests = {
        {
            name = "Stone",
            harvest = {
                Stone = 1
            }
        }
    }
}