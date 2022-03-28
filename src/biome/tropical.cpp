#include "tropical.hpp"

iso::Tropical::Tropical() 
    : Biome("Tropical", COLOUR_GREEN_TROPICAL) {}

iso::Tropical::~Tropical() {}

std::string iso::Tropical::getTile() const {
    return "tile_grass_tropical";
}

std::string iso::Tropical::getTree() const {
    const int type = rand() % 3;
    switch(type) {
        case 0:
            return "tile_tree_cypress";
        case 1:
            return "tile_tree_acacia";
        case 2:
            return "tile_tree_palm";
    }

    return "default";
}
