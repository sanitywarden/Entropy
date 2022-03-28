#include "mediterranean.hpp"

iso::Mediterranean::Mediterranean() 
    : Biome("Mediterranean", COLOUR_GREEN_MEDITERRANEAN) {}

iso::Mediterranean::~Mediterranean() {}

std::string iso::Mediterranean::getTile() const {
    return "tile_grass_subtropical";
}

std::string iso::Mediterranean::getTree() const {
    const int type = rand() % 2;
    switch(type) {
        case 0:
            return "tile_tree_cypress";
        case 1:
            return "tile_tree_acacia";
    }

    return "default";
}
