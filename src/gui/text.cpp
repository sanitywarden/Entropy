#include "text.hpp"

using namespace gui;

Label::Label() {
    this->m_text_alignment = sf::Vector2f(0, 0);
}

Label::Label(AbstractWidget* parent, sf::Text component, sf::Vector2f alignment) {
    if(alignment.x > 1.0f) alignment.x = 1.0f;
    if(alignment.x < 0.0f) alignment.x = 0.0f;
    if(alignment.y > 1.0f) alignment.y = 1.0f;
    if(alignment.y < 0.0f) alignment.y = 0.0f;
    
    this->m_text_component = component;
    this->m_text_alignment = alignment;

    this->setParent(parent);
}

Label::Label(AbstractWidget* parent, std::string text, sf::Font& font, unsigned int font_size, sf::Vector2f alignment) {
    if(alignment.x > 1.0f) alignment.x = 1.0f;
    if(alignment.x < 0.0f) alignment.x = 0.0f;
    if(alignment.y > 1.0f) alignment.y = 1.0f;
    if(alignment.y < 0.0f) alignment.y = 0.0f;

    this->m_text_alignment = alignment;
    this->m_text_component = sf::Text(text, font, font_size);

    this->setParent(parent);
}

Label::~Label() {
    
}

// Alignment is between 0 and 1 inclusive.
// point (0, 0) is the top left corner of the window.
void Label::align(sf::Vector2f alignment) {
    if(this->parent() == nullptr)
        return;
    
    if(alignment.x > 1.0f) alignment.x = 1.0f;
    if(alignment.x < 0.0f) alignment.x = 0.0f;
    if(alignment.y > 1.0f) alignment.y = 1.0f;
    if(alignment.y < 0.0f) alignment.y = 0.0f;


    sf::Vector2f label_position = sf::Vector2f(
        this->parent()->widgetPosition().x + this->parent()->widgetSize().x * alignment.x,  
        this->parent()->widgetPosition().y + this->parent()->widgetSize().y * alignment.y  
    );

    this->m_text_alignment = alignment;
    this->m_text_component.setPosition(label_position);
}

sf::Vector2f Label::alignment() const {
    return this->m_text_alignment;
}

sf::Text& Label::label() {
    return this->m_text_component;
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->parent() == nullptr)
        return;
           
    sf::Vector2f position = sf::Vector2f(
        this->parent()->widgetPosition().x + this->parent()->widgetSize().x * this->m_text_alignment.x,
        this->parent()->widgetPosition().y + this->parent()->widgetSize().y * this->m_text_alignment.y
    );

    this->m_text_component.setPosition(position);
    target.draw(this->m_text_component, states);
}
