Building = {
    name = "Gatherer",
    description = "Gatherer collects nearby herbs and plants",
    size = Vector2i(2, 2),
    texture = "building_gatherer",
    texture_size = Vector2i(128, 128),
    icon_texture = "icon_building_gatherer",
    icon_size = Vector2i(48, 48),
    removable = true,
    cost = {
        Wood = 10,
    },
    refund = {
        Wood = 5
    },
    scan_area = Vector2i(3, 3),
}