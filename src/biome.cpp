#include "biome.hpp"

using namespace iso;

Biome::Biome()
    : biome_name("Biomeless"), biome_colour(COLOUR_WHITE), biome_texture_tile(0), biome_texture_tree(0) {}

Biome::Biome(std::string name, sf::Color colour) 
    : biome_name(name), biome_colour(colour), biome_texture_tile(0), biome_texture_tree(0) {}

Biome::Biome(std::string name, sf::Color colour, std::vector <std::string> tileset, std::vector <std::string> treeset)
    : biome_name(name), biome_colour(colour), biome_texture_tile(tileset), biome_texture_tree(treeset) {}

Biome::~Biome() {
    
}

bool Biome::operator== (const Biome& biome) const {
    return this->biome_name == biome.biome_name;
}

std::string Biome::getTile() const {
    return this->biome_texture_tile[std::rand() % this->biome_texture_tile.size()];
}

std::string Biome::getTree() const {
    return this->biome_texture_tree[std::rand() % this->biome_texture_tree.size()];
}