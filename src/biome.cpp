#include "biome.hpp"

using namespace iso;

Biome::Biome() {
    this->biome_name    = "Biomeless";
    this->biome_climate = Climate::BIOMELESS;
    this->biome_colour  = COLOUR_WHITE;
}

Biome::Biome(std::string name, sf::Color biome_colour, Climate climate) {
    this->biome_name    = name;
    this->biome_climate = climate;
    this->biome_colour  = biome_colour;
}

Biome::~Biome() {

}

bool Biome::operator== (const Biome& biome) const {
    return this->biome_name == biome.biome_name;
}