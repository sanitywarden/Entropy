#include "image.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

ImageHolder::ImageHolder() {
    this->manager      = manager;
    this->texture_name = "*";
    std::cout << "[GUI][ImageHolder]: Simulation manager is a nullptr.\n";
}

ImageHolder::ImageHolder(SimulationManager* manager, std::string texture_name) {
    this->manager      = manager;
    this->texture_name = texture_name;
}

ImageHolder::~ImageHolder() {

}

void ImageHolder::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->manager == nullptr)
        return;

    auto texure_name = this->texture_name;
    auto image_size  = this->manager->resource.getTextureSize(texture_name);

    this->setWidgetSize(image_size);

    sf::VertexArray image(sf::Quads, 4);
    auto widget_position = this->getWidgetPosition() + this->getWidgetOffset();
    
    image[0].position = widget_position;
    image[1].position = widget_position + sf::Vector2f(image_size.x, 0);
    image[2].position = widget_position + sf::Vector2f(image_size.x, image_size.y);
    image[3].position = widget_position + sf::Vector2f(0, image_size.y);

    image[0].texCoords = sf::Vector2f(0, 0);
    image[1].texCoords = sf::Vector2f(image_size.x, 0);
    image[2].texCoords = sf::Vector2f(image_size.x, image_size.y);
    image[3].texCoords = sf::Vector2f(0, image_size.y);

    states.texture = &this->manager->resource.getTexture(texture_name);
    target.draw(image, states);
}

std::string ImageHolder::getTextureName() {
    return this->texture_name;
}
