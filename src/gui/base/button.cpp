#include "button.hpp"
#include <iostream>

using namespace gui;

Button::Button() {
    this->message.label().setString("[NO TEXT]");
    this->message.label().setFillColor(sf::Color::Black);
}

Button::Button(std::string message, sf::Font& font, unsigned int font_size) {
    this->message.label().setString(message);
    this->message.label().setFont(font);
    this->message.label().setCharacterSize(font_size);
    this->message.label().setFillColor(sf::Color::Black);
}

Button::~Button() {
    
} 

void Button::setTextComponent(const Label& text_component) {
    this->message = text_component;
}

Label& Button::textComponent() {
    return this->message;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Vector2f position = this->widgetPosition(); 
    sf::Vector2f size     = this->widgetSize();
    sf::Vector2f scale    = this->widgetScale();
    sf::Color    colour   = this->isTransparent() ? sf::Color(0, 0, 0, 0) : this->widgetColour();

    sf::Vector2f first_corner = sf::Vector2f(
        position.x,
        position.y
    ); 

    sf::VertexArray button(sf::Quads, 4);

    button[0].position = first_corner;
    button[1].position = first_corner + sf::Vector2f(size.x * scale.x, 0.f);
    button[2].position = first_corner + sf::Vector2f(size.x * scale.x, size.y * scale.y);
    button[3].position = first_corner + sf::Vector2f(0.f, size.y * scale.y);

    button[0].color = colour;
    button[1].color = colour;
    button[2].color = colour;
    button[3].color = colour;

    this->message.setWidgetPosition(position);

    target.draw(button, states);
    target.draw(this->message);
}

bool Button::containsPoint(sf::Vector2f point) {
    auto top_left     = this->widgetPosition();
    auto bottom_right = this->widgetPosition() + sf::Vector2f(this->widgetSize().x * this->widgetScale().x, this->widgetSize().y * this->widgetScale().y);
    
    if(point.x >= top_left.x && point.y >= top_left.y && point.x <= bottom_right.x && point.y <= bottom_right.y) 
        return true;
    return false;
}
