#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "biomeType.hpp"

namespace iso {
    class Region {
        private:

        public:
            Region();
            Region(BiomeType biome, float height, float moisture, float temperature, float latitude);
            ~Region();

            BiomeType biome;

            float height;
            float moisture;
            float temperature;
            float latitude;
    };
}

#endif