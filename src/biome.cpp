#include "biome.hpp"
#include "globalutilities.hpp"

using namespace iso;

Biome::Biome()
{}

Biome::Biome(const Biome& biome)
    : data(biome.data)
{}

Biome::Biome(const BiomeData& data)
    : data(data)
{}

Biome::~Biome()
{}

const std::string& Biome::getDefinitionFilename() const {
    return this->data.filename;
}

const std::string& Biome::getBiomeName() const {
    return this->data.name;
}

const std::string& Biome::getBiomeId() const {
    return this->data.id;
}

const std::string& Biome::getBiomeDescription() const {
    return this->data.description;
}

const sf::Color Biome::getBiomeWorldmapColour() const {
    return this->data.colour_wmap;
}

const sf::Color Biome::getBiomeRegionmapColour() const {
    return this->data.colour_rmap;
}

const std::string& Biome::getBiomeTemperature() const {
    return this->data.temperature;
}

const std::string& Biome::getBiomeMoisture() const {
    return this->data.moisture;
}

const std::string& Biome::getBiomeLatitude() const {
    return this->data.latitude;
}

const std::string& Biome::getBiomeElevation() const {
    return this->data.elevation;
}

const std::string& Biome::getWorldmapForestTexture() const {
    return this->data.forest_texture_worldmap;
}

const std::vector <std::string>& Biome::getBiomeTileList() const {
    return this->data.tiles;
}

const std::vector <std::string>& Biome::getBiomeTreeList() const {
    return this->data.trees;
}

const std::string& Biome::getRandomTile() const {
    return this->data.tiles[randomInclusiveBetween(0, this->data.tiles.size())];
}

const std::string& Biome::getRandomTree() const {
    return this->data.trees[randomInclusiveBetween(0, this->data.trees.size())];
}

bool Biome::canBeForest() const {
    return this->data.can_be_forest;
}