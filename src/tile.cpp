#include "tile.hpp"
#include "generationSettings.hpp"

using namespace iso;

Tile::Tile() {
    this->_marked = false;
    this->object_name = "Tile";
}

Tile::~Tile() {

}

void Tile::setElevation(int elevation) {
    this->object_position.z = -elevation * game_settings.tileSize().y / 2;
}

int Tile::getElevation() const {
    return -this->object_position.z / (game_settings.tileSize().y / 2);
}
