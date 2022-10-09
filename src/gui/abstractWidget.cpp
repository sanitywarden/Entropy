#include "abstractWidget.hpp"
#include "simulationManager.hpp"

#include <iostream>

namespace gui {
AbstractWidget::AbstractWidget() {
    this->data.widget_id     = "Undefined";
    this->data.position      = core::Vector2i(0, 0);
    this->data.size          = core::Vector2i(0, 0);
    this->data.parent        = nullptr;
    this->data.draw_priority = 0;
    this->data.draw          = false;
}

AbstractWidget::AbstractWidget(const WidgetData& data)
    : data(data)
{}

AbstractWidget::~AbstractWidget() 
{}

void AbstractWidget::setWidgetID(const std::string& id) {
    this->data.widget_id = id;
}

const std::string& AbstractWidget::getWidgetID() const {
    return this->data.widget_id;
}

void AbstractWidget::setWidgetPosition(core::Vector2i position) {
    this->data.position = position;
}

void AbstractWidget::setWidgetPosition(int x, int y) {
    this->data.position = core::Vector2i(x, y);
}

core::Vector2i AbstractWidget::getWidgetPosition() const {
    return this->data.position;
}

void AbstractWidget::setWidgetSize(core::Vector2i size) {
    this->data.size = size; 
}

void AbstractWidget::setWidgetSize(int x, int y) {
    this->data.size = core::Vector2i(x, y);
}

core::Vector2i AbstractWidget::getWidgetSize() const {
    return this->data.size;
}

void AbstractWidget::setParent(AbstractComponent parent){
    this->data.parent = parent;
}

AbstractWidget* const AbstractWidget::getParent() const {
    return this->data.parent.get();
}

bool AbstractWidget::hasParent() const {
    return this->data.parent != nullptr; 
}

bool AbstractWidget::containsPoint(core::Vector2i point) const {
    core::Vector2i top_left     = this->data.position;
    core::Vector2i bottom_right = this->data.position + this->data.size;
    return (point.x >= top_left.x && point.y >= top_left.y && point.x <= bottom_right.x && point.y <= bottom_right.y); 
}

void AbstractWidget::setVisible(bool draw) {
    this->data.draw = draw;
}

bool AbstractWidget::isVisible() const {
    return this->data.draw;
}

void AbstractWidget::toggleVisibile() {
    this->data.draw = !this->data.draw;
}

void AbstractWidget::setDrawPriority(int priority) {
    this->data.draw_priority = priority;
}

int AbstractWidget::getDrawPriority() const {
    return this->data.draw_priority;
}

void AbstractWidget::addEventOverride(const std::string& event_name) {
    this->data.event_overrides.push_back(event_name);
}

bool AbstractWidget::hasEventOverride(const std::string& event_name) const {
    return std::find(this->data.event_overrides.begin(), this->data.event_overrides.end(), event_name) != this->data.event_overrides.end();
}
}