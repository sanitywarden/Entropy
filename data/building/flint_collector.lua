Building = {
    name = "Flint Collector",
    description = "Flint collector collects small rocks",
    size = Vector2i(2, 2),
    texture = "building_flint_collector",
    texture_size = Vector2i(128, 128),
    icon_texture = "icon_building_flint_collector",
    icon_size = Vector2i(48, 48),
    removable = true,
    cost = {
        Wood = 8,
    },
    refund = {
        Wood = 2
    },
    scan_area = Vector2i(3, 3),
    harvests = {
        {
            name = "Flint",
            harvest = {
                Flint = 5
            }
        }
    }
}