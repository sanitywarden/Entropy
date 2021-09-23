#include "biome.hpp"

using namespace iso;

Biome::Biome() {
    this->biome_name    = "Biomeless";
    this->biome_climate = Climate::BIOMELESS;
}

Biome::Biome(std::string name, Climate climate) {
    this->biome_name    = name;
    this->biome_climate = climate;
}

Biome::~Biome() {

}