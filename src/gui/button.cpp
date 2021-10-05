#include "button.hpp"
#include <iostream>

using namespace gui;

Button::Button() {
    this->m_button_text.label().setString("");
}

Button::~Button() {
    
} 

void Button::setTextComponent(Label text_component) {
    this->m_button_text = text_component;
}

Label& Button::textComponent() {
    return this->m_button_text;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Vector2f position = this->widgetPosition(); 
    sf::Vector2f size     = this->widgetSize();
    sf::Vector2f scale    = this->widgetScale();
    sf::Color    colour   = this->isTransparent() ? sf::Color(0, 0, 0, 255) : this->widgetColour();

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

    this->m_button_text.setWidgetPosition(position);

    target.draw(button, states);
    target.draw(this->m_button_text);
}

bool Button::containsPoint(sf::Vector2f point) {
    auto top_left     = this->widgetPosition();
    auto bottom_right = this->widgetPosition() + sf::Vector2f(this->widgetSize().x * this->widgetScale().x, this->widgetSize().y * this->widgetScale().y);
    
    if(point.x >= top_left.x && point.y >= top_left.y && point.x <= bottom_right.x && point.y <= bottom_right.y) 
        return true;
    return false;
}

void Button::update(std::function<void()> callback) {
    callback();
}

void Button::onMouseButtonPress(std::function<void()> callback) {
    callback();
}

void Button::onMouseButtonRelease(std::function<void()> callback) {
    callback();
}

// void Button::alignTextComponent(TextAlignment alignment) {
    // if(alignment == TextAlignment::DEFAULT)
    //     return;
    
    // else if(alignment == TextAlignment::CENTRED) {
    //     this->m_button_text.setPosition(
    //         this->widget_position.x + this->widget_size.x / 2 - this->m_button_text.getGlobalBounds().width / 2,
    //         this->widget_position.y + this->widget_size.y / 2
    //     );

    //     this->m_button_text.setOrigin(
    //         this->m_button_text.getGlobalBounds().left + (float)this->m_button_text.getGlobalBounds().width  / 2.0f,
    //         this->m_button_text.getGlobalBounds().top  + (float)this->m_button_text.getGlobalBounds().height / 2.0f
    //     );
    // }

    // else if(alignment == TextAlignment::ALIGNED_LEFT) {

    // }

    // else if(alignment == TextAlignment::ALIGNED_RIGHT) {

    // }
// }