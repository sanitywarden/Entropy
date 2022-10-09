#include "tileable.hpp"

#include <cmath>

namespace gui {
void TileableBlock::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray block(sf::Quads, 4);

    auto position = this->position.asSFMLVector2f();
    auto size     = this->size.asSFMLVector2f();

    block[0].position = position;
    block[1].position = position + sf::Vector2f(size.x, 0);
    block[2].position = position + sf::Vector2f(size.x, size.y);
    block[3].position = position + sf::Vector2f(0, size.y);

    block[0].texCoords = sf::Vector2f(0, 0);
    block[1].texCoords = sf::Vector2f(size.x, 0);
    block[2].texCoords = sf::Vector2f(size.x, size.y);
    block[3].texCoords = sf::Vector2f(0, size.y);

    target.draw(block, states);
}

PropertyTileable::PropertyTileable(const std::string& base_texture, core::Vector2i dimensions) 
    : base_texture(base_texture), dimensions(dimensions), tiles(dimensions.x * dimensions.y)
{}

PropertyTileable::~PropertyTileable() 
{}

std::string PropertyTileable::getBlockTexture(int current_x, int current_y) const {
    return this->base_texture;
}

core::Vector2i PropertyTileable::getDimensions() const {
    return this->dimensions;
}
}