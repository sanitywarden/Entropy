#ifndef _BIOME_MEDITERRANEAN_HPP_
#define _BIOME_MEDITERRANEAN_HPP_

#include "biome.hpp"

namespace iso {
    class Mediterranean : public Biome {
        public:
            Mediterranean();
            ~Mediterranean();

            std::string getTile() const override;
            std::string getTree() const override;
    };
}

#endif