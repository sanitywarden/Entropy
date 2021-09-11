#include "region.hpp"

using namespace iso;

Region::Region() {
    this->biome       = BiomeType::BIOMELESS;
    this->height      = 0.0f;
    this->moisture    = 0.0f;
    this->temperature = 0.0f;
    this->latitude    = 0.0f;
}

Region::Region(BiomeType biome, float height, float moisture, float temperature, float latitude) {
    this->biome       = biome;
    this->height      = height;
    this->moisture    = moisture;
    this->temperature = temperature;
    this->latitude    = latitude;
}

Region::~Region() {
    
}