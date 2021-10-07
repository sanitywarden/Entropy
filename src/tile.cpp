#include "tile.hpp"

using namespace iso;

Tile::Tile() {
    this->position = sf::Vector2f(0, 0);
    this->size     = sf::Vector2f(0, 0);
    this->height   = 0;
}

Tile::~Tile() {

}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray tile(sf::Quads, 4);    

    tile[0].position = this->position;
    tile[1].position = this->position + sf::Vector2f(this->size.x, 0);
    tile[2].position = this->position + sf::Vector2f(this->size.x, this->size.y);
    tile[3].position = this->position + sf::Vector2f(0, this->size.y);

    tile[0].texCoords = sf::Vector2f(0, 0);
    tile[1].texCoords = sf::Vector2f(this->size.x, 0); 
    tile[2].texCoords = sf::Vector2f(this->size.x, this->size.y); 
    tile[3].texCoords = sf::Vector2f(0, this->size.y);

    states.texture = &this->texture;
    target.draw(tile, states); 

    if(this->side.exists())
        target.draw(this->side);

    if(this->tree.exists()) 
        target.draw(this->tree);
}