#include "tundra.hpp"

iso::Tundra::Tundra() 
    : Biome("Tundra", COLOUR_BROWN_TUNDRA) {}

iso::Tundra::~Tundra() {}

std::string iso::Tundra::getTile() const {
    return "tile_tundra";
}

std::string iso::Tundra::getTree() const {
    const int type = rand() % 2;
    switch(type) {
        case 0:
            return "tile_tree_spruce1";
        case 1:
            return "tile_tree_spruce2";
    }

    return "default";
}
