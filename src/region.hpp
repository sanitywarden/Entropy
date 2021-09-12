#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "biomeType.hpp"
#include "tile.hpp"

namespace iso {
    class Region {
        private:

        public:
            

        public:
            Region();
            Region(BiomeType biome, float height, float moisture, float temperature, float latitude, sf::Vector2i size);
            ~Region();

            BiomeType biome;

            float height;
            float moisture;
            float temperature;
            float latitude;
            
            sf::Vector2i map_size;
            std::vector <Tile> map;
    };
}

#endif