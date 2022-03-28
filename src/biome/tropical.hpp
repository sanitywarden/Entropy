#ifndef _BIOME_TROPICAL_HPP_
#define _BIOME_TROPICAL_HPP_

#include "biome.hpp"

namespace iso {
    class Tropical : public Biome {
        public:
            Tropical();
            ~Tropical();

            std::string getTile() const override;
            std::string getTree() const override;
    };
}

#endif