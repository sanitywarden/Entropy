#include "label.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

Label::Label() {
    this->manager = nullptr;
    this->data    = "*";
    std::cout << "[GUI][Label]: Simulation manager is a nullptr.\n";
}

Label::Label(SimulationManager* manager, std::string data) {
    this->manager = manager;
    this->data    = data;
}

Label::Label(SimulationManager* manager) {
    this->manager = manager;
}

Label::~Label() {
    
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->show)
        return;
    
    sf::Text text;
    text.setPosition(this->getWidgetPosition());
    text.setOrigin(this->final_origin);
    text.setFillColor(sf::Color::Black);
    text.setFont(this->manager->resource.getFont("garamond"));
    text.setString(this->data);
    text.setCharacterSize(14);

    target.draw(text, states);
}

void Label::align(Alignment alignment, sf::Vector2f parent_position, sf::Vector2f parent_size) {
    sf::Text text(this->data, this->manager->resource.getFont("garamond"), 14);
    sf::Rect text_rectangle = text.getLocalBounds();

    switch(alignment) {
        default: 
            break;

        case Alignment::ALIGNMENT_CENTRED: {
            this->final_origin   = sf::Vector2f(text_rectangle.left + text_rectangle.width / 2.0f, text_rectangle.top + text_rectangle.height / 2);
            this->final_position = sf::Vector2f(parent_position.x + parent_size.x / 4, parent_position.y + parent_size.y / 4);
            break;
        }

        case Alignment::ALIGMNENT_CENTRED_LEFT: {
            this->final_position = sf::Vector2f(parent_position.x, parent_position.y + parent_size.y / 4);
            break;
        }
    }
}

void Label::setString(std::string data) {
    this->data = data;
}