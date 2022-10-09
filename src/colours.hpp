#pragma once

#include <vector>

#include "types.hpp"

namespace iso {
    const core::Colour COLOUR_TRANSPARENT(0, 0, 0, 0);
    const core::Colour COLOUR_WHITE      (255, 255, 255, 255);
    const core::Colour COLOUR_BLACK      (0, 0, 0, 255);
    const core::Colour COLOUR_RED        (255, 20, 0, 255);
    const core::Colour COLOUR_GREEN      (0, 255, 0, 255);
    const core::Colour COLOUR_BLUE       (0, 0, 255, 255);
    const core::Colour COLOUR_YELLOW     (255, 255, 0, 255);

    const core::Colour COLOUR_WHITE_TRANSPARENT_HALF    (255, 255, 255, 127);
    const core::Colour COLOUR_WHITE_TRANSPARENT_QUARTER (255, 255, 255, 63);
    const core::Colour COLOUR_BLACK_TRANSPARENT_HALF    (0, 0, 0, 127);
    const core::Colour COLOUR_GREEN_TRANSPARENT_HALF    (46, 131, 31, 127);
    const core::Colour COLOUR_RED_TRANSPARENT_HALF      (229, 20, 0, 127);
    const core::Colour COLOUR_WHITE_TRANSPARENT90PERCENT(255, 255, 255, 255 * 0.9f);
    const core::Colour COLOUR_GREEN_TRANSPARENT90PERCENT(46, 131, 31, 255 * 0.9f);
    const core::Colour COLOUR_RED_TRANSPARENT90PERCENT  (229, 20, 0, 255 * 0.9f);

    // Biome specific colours.

    const core::Colour COLOUR_GREEN_TROPICAL     (79, 154, 22); 
    const core::Colour COLOUR_GREEN_MEDITERRANEAN(132, 177, 29);
    const core::Colour COLOUR_GREEN_TEMPERATE    (46, 131, 31);
    const core::Colour COLOUR_GREEN_CONTINENTAL  (24, 77, 14);
    const core::Colour COLOUR_BROWN_TUNDRA       (77, 77, 30);
    const core::Colour COLOUR_BLUE_OCEAN         (67, 102, 228);
    
    const core::Colour COLOUR_BLUE_RIVER(47, 154, 229);

    // Resource colours.

    const core::Colour COLOUR_GREY_STONE(103, 103, 103);
    const core::Colour COLOUR_GREY_FLINT(82, 82, 82);
}