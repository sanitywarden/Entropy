#include "biome.hpp"

using namespace iso;

Biome::Biome()
    : biome_name("Biomeless"), biome_colour(COLOUR_WHITE) {}

Biome::Biome(std::string name, sf::Color colour) 
    : biome_name(name), biome_colour(colour) {}

Biome::~Biome() {
    
}

bool Biome::operator== (const Biome& biome) const {
    return this->biome_name == biome.biome_name;
}