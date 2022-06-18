#include "tileable.hpp"

using namespace gui;

Block::Block()  {
    this->block_position     = sf::Vector2f(0, 0); 
    this->block_size         = sf::Vector2f(0, 0);
    this->block_texture_name = "*";
}

Block::~Block() {

}

void Block::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray block(sf::Quads, 4);

    block[0].position = this->block_position;
    block[1].position = this->block_position + sf::Vector2f(this->block_size.x, 0);
    block[2].position = this->block_position + sf::Vector2f(this->block_size.x, this->block_size.y);
    block[3].position = this->block_position + sf::Vector2f(0, this->block_size.y);

    block[0].texCoords = sf::Vector2f(0, 0);
    block[1].texCoords = sf::Vector2f(this->block_size.x, 0);
    block[2].texCoords = sf::Vector2f(this->block_size.x, this->block_size.y);
    block[3].texCoords = sf::Vector2f(0, this->block_size.y);

    target.draw(block, states);
}

PropertyTileable::PropertyTileable() {
}

PropertyTileable::~PropertyTileable() {

}

std::string PropertyTileable::getBlockTexture(int current_x, int current_y, sf::Vector2i dimensions) const {
    return this->m_texture;
}

sf::Vector2i PropertyTileable::getBlockSize() const {
    return this->m_dimensions;
}

void PropertyTileable::setBlockSize(sf::Vector2i size) {
    this->m_dimensions = size;
}

void PropertyTileable::setBlockTextureBase(std::string texture) {
    this->m_texture = texture;
}

std::string PropertyTileable::getBlockTextureBase() const {
    return this->m_texture;
}
