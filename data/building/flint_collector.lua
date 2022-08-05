Building = {
    name = "Flint Collector",
    description = "Flint collector collects small rocks",
    size = {
        x = 2,
        y = 2
    },
    texture = "building_flint_collector",
    texture_size = {
        x = 128,
        y = 128
    },
    icon_texture = "icon_building_flint_collector",
    icon_size = {
        x = 48,
        y = 48
    },
    removable = true,
    cost = {
        Wood = 8,
    },
    refund = {
        Wood = 2
    },
    scan_area = {
        x = 3,
        y = 3
    },
    harvests = {
        {
            name = "Flint",
            harvest = {
                Flint = 5
            }
        }
    }
}