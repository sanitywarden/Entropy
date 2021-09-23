#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "biome.hpp"
#include "tile.hpp"

namespace iso {
    class Region {
        public:
            Region();
            Region(Biome biome, float height, float moisture, float temperature, float latitude);
            ~Region();

            Biome biome;

            float height;
            float moisture;
            float temperature;
            float latitude;

            std::vector <Tile> map;
    };
}

#endif