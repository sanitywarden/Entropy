#include "widget.hpp"

using namespace gui;

Widget::Widget() {

}

Widget::~Widget() {

}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    auto position = this->widgetPosition(); 
    auto size     = this->widgetSize();
    auto scale    = this->widgetScale();
    auto colour   = this->isTransparent() ? sf::Color(0, 0, 0, 255) : this->widgetColour();

    sf::VertexArray widget(sf::Quads, 4);

    widget[0].position = position; 
    widget[1].position = position + sf::Vector2f(size.x * scale.x, 0.f);
    widget[2].position = position + sf::Vector2f(size.x * scale.x, size.y * scale.y);
    widget[3].position = position + sf::Vector2f(0.f, size.y * scale.y);

    widget[0].color = colour;
    widget[1].color = colour;
    widget[2].color = colour;
    widget[3].color = colour;

    target.draw(widget, states);  

    if(this->m_attached_components.size() > 0) {
        for(const auto& pair : this->m_attached_components) {
            auto* interface_component = pair.second;
            target.draw(*interface_component);
        }
    }
}

void Widget::attachComponent(gui::AbstractWidget* component, std::string component_name) {
    component->setWidgetPosition(this->widgetPosition() + component->widgetPosition());

    std::pair <std::string, gui::AbstractWidget*> pair = { component_name, component };

    this->m_attached_components.insert(pair);    
}

const gui::AbstractWidget& Widget::getComponentByName(std::string component_name) const {
    // This should definately have a value.
    // Although if you read this message, and have a access violation error perhaps this might be the reason, because you dereference a nullptr; 
    return *this->m_attached_components.at(component_name);
}

bool Widget::containsPoint(sf::Vector2f point) {
    auto top_left     = this->widgetPosition();
    auto bottom_right = this->widgetPosition() + sf::Vector2f(this->widgetSize().x * this->widgetScale().x, this->widgetSize().y * this->widgetScale().y);
    
    if(point.x >= top_left.x && point.y >= top_left.y && point.x <= bottom_right.x && point.y <= bottom_right.y) 
        return true;
    return false;
}
