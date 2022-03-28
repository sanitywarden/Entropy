#ifndef _BIOME_TUNDRA_HPP_
#define _BIOME_TUNDRA_HPP_

#include "biome.hpp"

namespace iso {
    class Tundra : public Biome {
        public:
            Tundra();
            ~Tundra();

            std::string getTile() const override;
            std::string getTree() const override;
    };
}

#endif