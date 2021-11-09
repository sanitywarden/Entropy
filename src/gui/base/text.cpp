#include "text.hpp"

using namespace gui;

Label::Label() {

}

Label::Label(AbstractWidget* parent, sf::Text component) {
    this->text = component;
    this->setParent(parent);
}

Label::Label(AbstractWidget* parent, std::string text, sf::Font& font, unsigned int font_size) {
    this->text = sf::Text(text, font, font_size);
    this->setParent(parent);
}

Label::~Label() {
    
}

void Label::align(Alignment alignment, sf::Vector2f offset) {
    const sf::Vector2f  parent_position = this->parent()->widgetPosition();
    const sf::Vector2f  parent_size     = this->parent()->widgetSize();
    const sf::Vector2f  text_position   = this->text.getPosition();
    const sf::FloatRect text_rectangle  = this->text.getLocalBounds();

    switch(alignment) {
        default: break;

        case Alignment::ALIGNMENT_CENTRED: {
            this->text.setOrigin(text_rectangle.left + text_rectangle.width / 2.0f, text_rectangle.top + text_rectangle.height / 2);
            this->text.setPosition(parent_position.x + parent_size.x / 2, parent_position.y + parent_size.y / 2);
            break;
        }
    }

    if(offset.x != 0.f) {
        this->text.move(parent_size.x * offset.x, 0);
    }

    if(offset.y != 0.f) {
        this->text.move(0, parent_size.y * offset.y);
    }
}

sf::Text& Label::label() {
    return this->text;
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->parent() == nullptr)
        return;

    this->text.setFillColor(this->widgetColour());

    target.draw(this->text, states);
}
