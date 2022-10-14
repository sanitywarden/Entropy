#include "image.hpp"
#include "simulationManager.hpp"

namespace gui {
Image::Image(const WidgetData& data, const std::string& texture_id)
    : AbstractWidget(data), texture(texture_id)
{}

Image::~Image()
{}

void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray image(sf::Quads, 4);

    auto position = this->getWidgetPosition().asSFMLVector2f();
    auto size     = this->getImageSize().asSFMLVector2f();

    image[0].position = position;
    image[1].position = position + sf::Vector2f(size.x, 0);
    image[2].position = position + sf::Vector2f(size.x, size.y);
    image[3].position = position + sf::Vector2f(0, size.y);

    image[0].texCoords = sf::Vector2f(0, 0);
    image[1].texCoords = sf::Vector2f(size.x, 0);
    image[2].texCoords = sf::Vector2f(size.x, size.y);
    image[3].texCoords = sf::Vector2f(0, size.y);

    states.texture = &game_manager.resource.getTexture(this->texture);
    target.draw(image, states);
}

const std::string& Image::getImageTexture() const {
    return this->texture;
}

void Image::setImageTexture(const std::string& texture_id) {
    this->texture = texture_id;
}

core::Vector2f Image::getImageSize() const {
    return game_manager.resource.getTextureSize(this->texture);
}
}