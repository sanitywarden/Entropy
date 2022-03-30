#include "tile.hpp"

using namespace iso;

Tile::Tile() {
    this->_marked = false;
    this->elevation = 0;
}

Tile::~Tile() {

}

sf::Vector2f Tile::getTransformedPosition() const {
    return this->getPosition() + sf::Vector2f(0, -this->elevation * this->getSize().y / 2);
}