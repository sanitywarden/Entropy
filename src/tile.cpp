#include "tile.hpp"
#include "generationSettings.hpp"

using namespace iso;

Tile::Tile() {
    this->_marked = false;
    this->resource = nullptr;
}

Tile::~Tile() {

}

void Tile::setElevation(int elevation) {
    this->object_position.z = -elevation * world_settings.tileSize().y / 2;
}

int Tile::getElevation() const {
    return -this->object_position.z / (world_settings.tileSize().y / 2);
}

bool Tile::hasResource() const {
    return this->resource.get() != nullptr;
}

const Resource* Tile::getResource() const {
    return this->resource.get();
}