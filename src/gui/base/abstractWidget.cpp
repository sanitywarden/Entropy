#include "abstractWidget.hpp"

using namespace gui;

AbstractWidget::AbstractWidget() {
    this->setWidgetPosition(sf::Vector2f(0.f, 0.f));
    this->setWidgetSize(sf::Vector2f(0.f, 0.f));
    this->setWidgetScale(sf::Vector2f(1.f, 1.f));
    this->setWidgetID("NULL");
    this->setTransparent(false);
    
    this->m_parent = nullptr;
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

void AbstractWidget::setParent(AbstractWidget* parent) {
    this->m_parent = parent;
}

// Returns a parent of the GUI component (return value of AbstractWidget::attached() is true).
// Returns a nullptr if parent does not exist (return value of AbstractWidget::attached() is false). 
AbstractWidget* AbstractWidget::parent() const {
    return this->m_parent;
}

// Check if the GUI component is attached to a high-level entity.
bool AbstractWidget::attached() const {
    return (this->m_parent) ? true : false; 
}

// This callback has to handle everything that can happen with a component.
void AbstractWidget::update() {
    this->m_update_callback();
}

void AbstractWidget::setCallback(std::function <void()> callback) {
    this->m_update_callback = callback;
}