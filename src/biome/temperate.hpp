#ifndef _BIOME_TEMPERATE_HPP_
#define _BIOME_TEMPERATE_HPP_

#include "biome.hpp"

namespace iso {
    class Temperate : public Biome {
        public:
            Temperate();
            ~Temperate();

            std::string getTile() const override;
            std::string getTree() const override;
    };
}

#endif