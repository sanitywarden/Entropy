#ifndef _BIOME_CONTINENTAL_HPP_
#define _BIOME_CONTINENTAL_HPP_

#include "biome.hpp"

namespace iso {
    class Continental : public Biome {
        public:
            Continental();
            ~Continental();

            std::string getTile() const override;
            std::string getTree() const override;
    };
}

#endif