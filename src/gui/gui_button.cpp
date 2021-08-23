#include "gui_button.hpp"

gui::Button::Button() {
    this->m_button_shape = sf::VertexArray(sf::Quads, 4);

    this->m_is_transparent = false;
}

gui::Button::~Button() {
    
} 

void gui::Button::setTextComponent(const sf::Text& text_component) {
    this->m_button_text = text_component;
}

const sf::Text& gui::Button::getTextComponent() {
    return this->m_button_text;
}

void gui::Button::setTransparent(const bool& transparency) {
    this->m_is_transparent = transparency;
}

const bool& gui::Button::isTransparent() {
    return this->m_is_transparent;
}

void gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    auto first_corner = sf::Vector2f(
        this->widget_position.x,
        this->widget_position.y
    ); 

    this->m_button_shape[0].position = first_corner;
    this->m_button_shape[1].position = first_corner + sf::Vector2f(this->widget_size.x, 0.f);
    this->m_button_shape[2].position = first_corner + sf::Vector2f(this->widget_size.x, this->widget_size.y);
    this->m_button_shape[3].position = first_corner + sf::Vector2f(0.f, this->widget_size.y);

    if(this->m_is_transparent) {
        target.draw(this->m_button_shape);
    }

    if(this->m_button_text.getFont()) {
        target.draw(this->m_button_text);
    }

    else std::cout << "[gui::Button]: Font not assigned. Text will not be displayed.\n";
}

bool gui::Button::containsPoint(const sf::Vector2f& point) {
    auto top_left = this->widget_position;
    auto bottom_right = this->widget_position + this->widget_size;
    
    if(point.x >= top_left.x && point.y >= top_left.y && point.x <= bottom_right.x && point.y <= bottom_right.y) return true;
    return false;
}

void gui::Button::onMouseButtonPress(std::function<void()> callback) {
    callback();
}

void gui::Button::onMouseButtonRelease(std::function<void()> callback) {
    callback();
}