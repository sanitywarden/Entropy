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

    this->text = sf::Text(data, this->manager->resource.getFont("garamond"), this->manager->font_size);
    this->text.setFillColor(sf::Color::Black);
    this->alignment = Alignment::ALIGNMENT_NONE;
}

Label::Label(SimulationManager* manager) {
    this->manager = manager;

    this->text = sf::Text("", this->manager->resource.getFont("garamond"), this->manager->font_size);
    this->text.setFillColor(sf::Color::Black);
    this->alignment = Alignment::ALIGNMENT_NONE;
}

Label::~Label() {
    
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->show)
        return;
    
    if(this->alignment != Alignment::ALIGNMENT_NONE) {
        this->text.setPosition(this->final_position);
        this->text.setOrigin(this->final_origin);
    }

    else
        this->text.setPosition(this->getWidgetPosition());

    this->text.setCharacterSize(this->manager->font_size);    
    this->text.setString(this->data);

    target.draw(text, states);
}

void Label::align(Alignment alignment, const AbstractWidget* parent) {
    if(!parent)
        return;

    this->alignment = alignment;

    sf::Rect tr_local  = text.getLocalBounds();
    sf::Rect tr_global = text.getGlobalBounds(); 

    auto parent_position = parent->getWidgetPosition();
    auto parent_size     = parent->getWidgetSize();

    switch(alignment) {
        default: 
            break;

        case Alignment::ALIGNMENT_CENTRED: {
            this->final_origin   = sf::Vector2f(tr_local.left + tr_local.width / 2.0f, tr_local.top + tr_local.height / 2);
            this->final_position = sf::Vector2f(parent_position.x + parent_size.x / 2, parent_position.y + parent_size.y / 2);            
            break;
        }

        case Alignment::ALIGMNENT_CENTRED_LEFT: {
            this->final_origin   = sf::Vector2f(tr_local.left, tr_local.top + tr_local.height / 2);
            this->final_position = sf::Vector2f(parent_position.x + 8, parent_position.y + parent_size.y / 2);    
            break;
        }

        case Alignment::ALIGMNENT_CENTRED_TOP: {
            this->final_origin   = sf::Vector2f(tr_local.left + tr_local.width / 2.0f, tr_local.top);
            this->final_position = sf::Vector2f(parent_position.x + parent_size.x / 2, parent_position.y + 8);
            break;
        }
    }
}

void Label::setString(std::string data) {
    this->data = data;
}

void Label::setColour(sf::Color colour) {
    this->text.setFillColor(colour);
}