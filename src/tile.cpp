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

bool Tile::is_terrain() const {
    return this->tiletype.is_terrain();
}

bool Tile::is_river() const {
    return this->tiletype.is_river();
}

bool Tile::is_ocean() const {
    return this->tiletype.is_ocean();
} 

bool Tile::is_water() const {
    return this->is_river() || this->is_ocean();
}

bool Tile::is_coast() const {
    return this->tiletype.is_coast();
}
