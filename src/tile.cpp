#include "tile.hpp"
#include "generationSettings.hpp"

using namespace iso;

Tile::Tile() {
    this->_marked = false;
}

Tile::~Tile() {

}

void Tile::setElevation(int elevation) {
    this->object_position.z = -elevation * world_settings.tileSize().y / 2;
}

int Tile::getElevation() const {
    return -this->object_position.z / (world_settings.tileSize().y / 2);
}