#include "tile.hpp"

using namespace iso;

Tile::Tile() {
    this->_marked = false;
    this->elevation = 0;
}

Tile::~Tile() {

}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray tile(sf::Quads, 4);    

    tile[0].position = this->getPosition();
    tile[1].position = this->getPosition() + sf::Vector2f(this->getSize().x, 0);
    tile[2].position = this->getPosition() + sf::Vector2f(this->getSize().x, this->getSize().y);
    tile[3].position = this->getPosition() + sf::Vector2f(0, this->getSize().y);

    tile[0].texCoords = sf::Vector2f(0, 0);
    tile[1].texCoords = sf::Vector2f(this->getSize().x, 0); 
    tile[2].texCoords = sf::Vector2f(this->getSize().x, this->getSize().y); 
    tile[3].texCoords = sf::Vector2f(0, this->getSize().y);

    target.draw(tile, states);
}
