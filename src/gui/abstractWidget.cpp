#include "abstractWidget.hpp"

using namespace gui;

AbstractWidget::AbstractWidget() {
    this->widget_position = sf::Vector2f(0.f, 0.f);
    this->widget_size     = sf::Vector2f(0.f, 0.f);
    this->widget_scale    = sf::Vector2f(1.f, 1.f);

    this->m_widget_id = "[]";
}

AbstractWidget::~AbstractWidget() {
    
}

void AbstractWidget::setWidgetID(std::string& id) {
    this->m_widget_id = id;
}

std::string& AbstractWidget::getWidgetID() {
    return this->m_widget_id;
}
