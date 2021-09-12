#include "region.hpp"

using namespace iso;

Region::Region() {
    this->biome       = BiomeType::BIOMELESS;
    this->height      = 0.0f;
    this->moisture    = 0.0f;
    this->temperature = 0.0f;
    this->latitude    = 0.0f;

    this->map_size = sf::Vector2i(0, 0);
}

Region::Region(BiomeType biome, float height, float moisture, float temperature, float latitude, sf::Vector2i size) {
    this->biome       = biome;
    this->height      = height;
    this->moisture    = moisture;
    this->temperature = temperature;
    this->latitude    = latitude;

    this->map_size = size;
}

Region::~Region() {
    
}