#include "continental.hpp"

iso::Continental::Continental() 
    : Biome("Continental", COLOUR_GREEN_CONTINENTAL) {}

iso::Continental::~Continental() {}

std::string iso::Continental::getTile() const {
    return "tile_grass_cold";
}

std::string iso::Continental::getTree() const {
    const int type = rand() % 2;
    switch(type) {
        case 0:
            return "tile_tree_spruce1";
        case 1:
            return "tile_tree_spruce2";
    }

    return "default";
}
