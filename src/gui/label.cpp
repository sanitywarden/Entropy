#include "label.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

Label::Label() {
    this->manager = nullptr;
    this->m_data  = "*";
    std::cout << "[GUI][Label]: Simulation manager is a nullptr.\n";
}

Label::Label(SimulationManager* manager, std::string data) {
    this->manager = manager;
    this->m_data  = data;
}

Label::Label(SimulationManager* manager) {
    this->manager = manager;
}

Label::~Label() {
    
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->show)
        return;
    
    this->m_text.setPosition(this->getWidgetPosition());
    this->m_text.setCharacterSize(14);
    this->m_text.setFillColor(sf::Color::Black);
    this->m_text.setFont(this->manager->resource.getFont("garamond"));
    this->m_text.setString(this->m_data);

    target.draw(this->m_text, states);
}

void Label::align(Alignment alignment, sf::Vector2f parent_position, sf::Vector2f parent_size, sf::Vector2f offset) {
    sf::FloatRect text_rectangle  = this->m_text.getLocalBounds();

    switch(alignment) {
        default: 
            break;

        case Alignment::ALIGNMENT_CENTRED: {
            this->m_text.setOrigin(text_rectangle.left + text_rectangle.width / 2.0f, text_rectangle.top + text_rectangle.height / 2);
            this->m_text.setPosition(parent_position.x + parent_size.x / 4, parent_position.y + parent_size.y / 4);
            break;
        }
    }

    if(offset.x != 0.f) {
        this->m_text.move(parent_size.x * offset.x, 0);
    }

    if(offset.y != 0.f) {
        this->m_text.move(0, parent_size.y * offset.y);
    }
}

sf::Text& Label::label() {
    return this->m_text;
}

void Label::setString(std::string data) {
    this->m_data = data;
}