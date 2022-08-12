Building = {
    name = "Hunter",
    description = "Hunter provides meat and leather from animals",
    size = Vector2i(2, 2),
    texture = "building_hunter",
    texture_size = Vector2i(128, 128),
    icon_texture = "icon_building_hunter",
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
            name = "Animal",
            harvest = {
                Meat = 4,
                Leather = 2
            }
        }
    }
}