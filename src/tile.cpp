#include "tile.hpp"

using namespace iso;

Tile::Tile() {

}

Tile::~Tile() {

}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray tile(sf::Quads, 4);

    auto tile_position = sf::Vector2f(
        this->tile_position.x,
        this->tile_position.y
    );

    tile[0].position = tile_position;
    tile[1].position = tile_position + sf::Vector2f(this->tile_size.x, 0);
    tile[2].position = tile_position + sf::Vector2f(this->tile_size.x, this->tile_size.y);
    tile[3].position = tile_position + sf::Vector2f(0, this->tile_size.y);

    tile[0].texCoords = sf::Vector2f(0, 0);
    tile[1].texCoords = sf::Vector2f(this->tile_size.x, 0); 
    tile[2].texCoords = sf::Vector2f(this->tile_size.x, this->tile_size.y); 
    tile[3].texCoords = sf::Vector2f(0, this->tile_size.y);

    states.texture = &this->tile_texture;

    target.draw(tile, states); 
}