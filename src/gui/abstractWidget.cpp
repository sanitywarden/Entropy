#include "abstractWidget.hpp"

#include <iostream>

using namespace gui;

AbstractWidget::AbstractWidget() {
    this->m_widget_id       = "*";
    this->m_widget_position = sf::Vector2f(0, 0);
    this->m_widget_size     = sf::Vector2f(0, 0);
    this->m_parent          = nullptr;
    this->m_adjust          = true;
    this->m_transparent     = false;
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

void AbstractWidget::setWidgetPosition(float x, float y) {
    this->m_widget_position = sf::Vector2f(x, y);
}

sf::Vector2f AbstractWidget::getWidgetPosition() const {
    return this->m_widget_position;
}

void AbstractWidget::setWidgetSize(sf::Vector2f size) {
    this->m_widget_size = size; 
}

void AbstractWidget::setWidgetSize(float x, float y) {
    this->m_widget_size = sf::Vector2f(x, y);
}

sf::Vector2f AbstractWidget::getWidgetSize() const {
    return this->m_widget_size;
}

void AbstractWidget::setTransparent(bool transparent) {
    this->m_transparent = transparent;
}

bool AbstractWidget::isTransparent() const {
    return this->m_transparent;
}

void AbstractWidget::setParent(AbstractWidget* parent){
    this->m_parent = parent;
}

// Returns a parent of the GUI component (return value of AbstractWidget::attached() is true).
// Returns a nullptr if parent does not exist (return value of AbstractWidget::attached() is false). 
AbstractWidget* AbstractWidget::getParent() const {
    return this->m_parent;
}

// Check if the GUI component is attached to a high-level entity.
bool AbstractWidget::attached() const {
    return (this->m_parent) ? true : false; 
}

sf::Vector2f AbstractWidget::getFinalPosition(const AbstractWidget* parent) const {
    sf::Vector2f final_position = parent
        ? parent->getWidgetPosition()
        : sf::Vector2f(0, 0);
        
    if(parent != nullptr)
        final_position += this->getFinalPosition(parent->getParent());

    return final_position;
}

bool AbstractWidget::containsPoint(sf::Vector2f point) const {
    auto top_left     = this->getWidgetPosition();
    auto bottom_right = this->getWidgetPosition() + this->getWidgetSize();

    return (point.x >= top_left.x && point.y >= top_left.y && point.x <= bottom_right.x && point.y <= bottom_right.y); 
}

void AbstractWidget::setAdjustable(bool adjust) {
    this->m_adjust = adjust;
}

bool AbstractWidget::isAdjustable() const {
    return this->m_adjust;
}