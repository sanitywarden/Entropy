#include "tile.hpp"
#include "worldData.hpp"

using namespace iso;

Tile::Tile() {
    this->_marked = false;
    this->object_name = "Tile";
}

Tile::~Tile() {

}

void Tile::setElevation(int elevation) {
    this->object_position.z = -elevation * tileSize().y / 2;
}

int Tile::getElevation() const {
    return -this->object_position.z / (tileSize().y / 2);
}

bool Tile::isTerrain() const {
    return this->tiletype.is_terrain();
}

bool Tile::isRiver() const {
    return this->tiletype.is_river();
}

bool Tile::isOcean() const {
    return this->tiletype.is_ocean();
} 

bool Tile::isWater() const {
    return this->isRiver() || this->isOcean();
}

bool Tile::isCoast() const {
    return this->tiletype.is_coast();
}
