#include "guiutilities.hpp"
#include "simulationManager.hpp"

namespace gui {
core::Vector2i calculateTileableDimensions(core::Vector2i tl_corner_position, core::Vector2i br_corner_position, int block_size) {
    auto distance = tl_corner_position - br_corner_position;
    if(distance.x < 0) distance.x = -distance.x;
    if(distance.y < 0) distance.y = -distance.y;

    auto ratio = (float)game_manager.window.getWindowWidth() / (float)game_manager.window.getWindowHeight();
    auto block_x = std::ceil(block_size * ratio);
    auto block_y = block_size;

    auto dimensions = core::Vector2i(
        std::ceil(distance.x / block_x) + 1,
        std::ceil(distance.y / block_y) + 1
    );

    return dimensions;
}

core::Vector2i calculateSizeFromDimensions(core::Vector2i dimensions, int block_size) {
    auto size = core::Vector2i(
        dimensions.x * block_size,
        dimensions.y * block_size
    );
    
    return size;
}

core::Vector2i calculateRelativeWidgetPosition(core::Vector2f point_ratio, AbstractComponent parent) {
    if(parent == nullptr) {
        auto position = core::Vector2i(
            std::ceil(point_ratio.x * game_manager.window.getWindowWidth()),
            std::ceil(point_ratio.y * game_manager.window.getWindowHeight())
        );

        return position; 
    }

    auto parent_tl_corner = parent.get()->getWidgetPosition();
    auto parent_size = parent.get()->getWidgetSize();
    auto position = parent_tl_corner + core::Vector2i(
        std::ceil(point_ratio.x * parent_size.x),
        std::ceil(point_ratio.y * parent_size.y)
    );
    
    return position;
}
}