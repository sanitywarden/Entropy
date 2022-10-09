#include "biome.hpp"
#include "globalutilities.hpp"

namespace iso {
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

const core::Colour Biome::getBiomeWorldmapColour() const {
    return this->data.colour_wmap;
}

const std::string& Biome::getBiomeTemperature() const {
    return this->data.temperature;
}

const std::string& Biome::getBiomeMoisture() const {
    return this->data.moisture;
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

void Biome::setBiomeName(const std::string& name) {
    this->data.name = name;
}   

void Biome::setBiomeId(const std::string& id) {
    this->data.id = id;    
}

void Biome::setBiomeDescription(const std::string& description) {
    this->data.description = description;
}

void Biome::setBiomeWorldmapColour(core::Colour colour) {
    this->data.colour_wmap = colour;
}

void Biome::setBiomeTemperature(const std::string& temperature) {
    this->data.temperature = temperature;
}

void Biome::setBiomeMoisture(const std::string& moisture) {
    this->data.moisture = moisture;
}

void Biome::setWorldmapForestTexture(const std::string& texture_name) {
    this->data.forest_texture_worldmap = texture_name;
}

void Biome::setBiomeTileList(std::vector <std::string> list) {
    this->data.tiles = list;
}

void Biome::setBiomeTreeList(std::vector <std::string> list) {
    this->data.trees = list;
}

bool Biome::operator== (const Biome& biome) const {
    return this->getBiomeId() == biome.getBiomeId();
}
}