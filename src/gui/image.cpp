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

    sf::VertexArray image(sf::Quads, 4);
    sf::Vector2f widget_position = this->getWidgetPosition();
    sf::Vector2f widget_size     = this->getWidgetSize();
    
    image[0].position = widget_position;
    image[1].position = widget_position + sf::Vector2f(widget_size.x, 0);
    image[2].position = widget_position + sf::Vector2f(widget_size.x, widget_size.y);
    image[3].position = widget_position + sf::Vector2f(0, widget_size.y);

    image[0].texCoords = sf::Vector2f(0, 0);
    image[1].texCoords = sf::Vector2f(widget_size.x, 0);
    image[2].texCoords = sf::Vector2f(widget_size.x, widget_size.y);
    image[3].texCoords = sf::Vector2f(0, widget_size.y);

    states.texture = &this->manager->resource.getTexture(this->texture_name);
    target.draw(image, states);
}

std::string ImageHolder::getTextureName() {
    return this->texture_name;
}