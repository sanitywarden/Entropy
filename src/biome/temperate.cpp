#include "temperate.hpp"

iso::Temperate::Temperate() 
    : Biome("Temperate", COLOUR_GREEN_TEMPERATE) {}

iso::Temperate::~Temperate() {}

std::string iso::Temperate::getTile() const {
    return "tile_grass_warm";
}

std::string iso::Temperate::getTree() const {
    const int type = rand() % 2;
    switch(type) {
        case 0:
            return "tile_tree_beech";
        case 1:
            return "tile_tree_oak";
    }

    return "default";
}
