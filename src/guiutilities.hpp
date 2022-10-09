#pragma once

#include "types.hpp"
#include "abstractWidget.hpp"

namespace gui {
core::Vector2i calculateTileableDimensions(core::Vector2i tl_corner_position, core::Vector2i br_corner_position, int block_size);
core::Vector2i calculateSizeFromDimensions(core::Vector2i dimensions, int block_size);
core::Vector2i calculateRelativeWidgetPosition(core::Vector2f point_ratio, AbstractComponent parent); 
}