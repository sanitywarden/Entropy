#ifndef _BIOME_DEFINITIONS_HPP_
#define _BIOME_DEFINITIONS_HPP_

#include "biome.hpp"
#include "temperate.hpp"
#include "continental.hpp"
#include "mediterranean.hpp"
#include "tropical.hpp"
#include "tundra.hpp"

namespace iso {
    static Biome         BIOME_OCEAN("Ocean", COLOUR_BLUE_OCEAN); // Since this biome is not possible to visit, it does not need a separate class.
    static Temperate     BIOME_TEMPERATE;
    static Continental   BIOME_CONTINENTAL;
    static Mediterranean BIOME_MEDITERRANEAN;
    static Tropical      BIOME_TROPICAL;
    static Tundra        BIOME_TUNDRA;
}

#endif