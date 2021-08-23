#include "gui_header.hpp"

gui::Header::Header() {}

gui::Header::~Header() {}

void gui::Header::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->m_label.getFont()) {
        target.draw(this->m_label);
    }

    else std::cout << "[gui::Header]: Font not assigned. Text will not be displayed.\n";
}

void gui::Header::setTextComponent(const sf::Text& text_component) {
    this->m_label = text_component;
}

const sf::Text& gui::Header::getTextComponent() {
    return this->m_label;
}