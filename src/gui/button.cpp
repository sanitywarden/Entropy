#include "button.hpp"

using namespace gui;

Button::Button() {
    this->m_transparent  = false;
    this->colour = sf::Color(127, 127, 127);
}

Button::~Button() {
    
} 

void Button::setTextComponent(sf::Text& text_component) {
    this->m_button_text = text_component;
}

sf::Text& Button::getTextComponent() {
    return this->m_button_text;
}

void Button::setTransparent(bool& transparency) {
    this->m_transparent = transparency;
}

bool& Button::isTransparent() {
    return this->m_transparent;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    auto first_corner = sf::Vector2f(
        this->widget_position.x,
        this->widget_position.y
    ); 

    this->m_button_shape = sf::VertexArray(sf::Quads, 4);

    this->m_button_shape[0].position = first_corner;
    this->m_button_shape[1].position = first_corner + sf::Vector2f(this->widget_size.x, 0.f);
    this->m_button_shape[2].position = first_corner + sf::Vector2f(this->widget_size.x, this->widget_size.y);
    this->m_button_shape[3].position = first_corner + sf::Vector2f(0.f, this->widget_size.y);

    this->m_button_shape[0].color = this->colour;
    this->m_button_shape[1].color = this->colour;
    this->m_button_shape[2].color = this->colour;
    this->m_button_shape[3].color = this->colour;

    target.draw(this->m_button_shape, states);
}

bool Button::containsPoint(const sf::Vector2f& point) {
    auto top_left = this->widget_position;
    auto bottom_right = this->widget_position + this->widget_size;
    
    if(point.x >= top_left.x && point.y >= top_left.y && point.x <= bottom_right.x && point.y <= bottom_right.y) return true;
    return false;
}

void Button::onMouseButtonPress(std::function<void()> callback) {
    callback();
}

void Button::onMouseButtonRelease(std::function<void()> callback) {
    callback();
}