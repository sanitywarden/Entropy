#include "abstractWidget.hpp"

using namespace gui;

AbstractWidget::AbstractWidget() {
    this->setWidgetPosition(sf::Vector2f(0.f, 0.f));
    this->setWidgetSize(sf::Vector2f(0.f, 0.f));
    this->setWidgetScale(sf::Vector2f(1.f, 1.f));
    this->setWidgetID("NULL");
    this->setTransparent(false);
}

AbstractWidget::~AbstractWidget() {
    
}

void AbstractWidget::setWidgetID(std::string id) {
    this->m_widget_id = id;
}

std::string AbstractWidget::getWidgetID() const {
    return this->m_widget_id;
}

void AbstractWidget::setWidgetPosition(sf::Vector2f position) {
    this->m_widget_position = position;
}

sf::Vector2f AbstractWidget::widgetPosition() const {
    return this->m_widget_position;
}

void AbstractWidget::setWidgetSize(sf::Vector2f size) {
    this->m_widget_size = size; 
}

sf::Vector2f AbstractWidget::widgetSize() const {
    return this->m_widget_size;
}

void AbstractWidget::setWidgetScale(sf::Vector2f scale) {
    this->m_widget_scale = scale;
} 

sf::Vector2f AbstractWidget::widgetScale() const {
    return this->m_widget_scale;
}

void AbstractWidget::setWidgetColour(sf::Color colour) {
    this->m_widget_colour = colour;
}

sf::Color AbstractWidget::widgetColour() const {
    return this->m_widget_colour;
}

void AbstractWidget::setTransparent(bool transparent) {
    this->m_transparent = transparent;
}

bool AbstractWidget::isTransparent() const {
    return this->m_transparent;
}