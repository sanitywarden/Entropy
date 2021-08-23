#include "gui_widget.hpp"

gui::Widget::Widget() {
    this->widget_position = sf::Vector2f(0.f, 0.f);
    this->widget_size     = sf::Vector2f(100.f, 100.f);
    this->widget_scale    = sf::Vector2f(1.f, 1.f);

    this->widget_id = "";
}

gui::Widget::~Widget() {
    
}

void gui::Widget::setWidgetID(const std::string& id) {
    this->widget_id = id;
}

const std::string& gui::Widget::getWidgetID() {
    return this->widget_id;
}

