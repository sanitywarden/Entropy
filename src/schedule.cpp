#include "schedule.hpp"

iso::ScheduledEvent::ScheduledEvent(const EventData& data)
    : data(data)
{}

iso::ScheduledEvent::~ScheduledEvent()
{}

void iso::ScheduledEvent::resetTime() {
    this->data.time = 0;
}

void iso::ScheduledEvent::progressEvent() {
    this->data.time++;
}

const std::string& iso::ScheduledEvent::getEventName() const {
    return this->data.name;
}

int iso::ScheduledEvent::getRequiredTime() const {
    return this->data.required_time;
}

int iso::ScheduledEvent::getCurrentTime() const {
    return this->data.time;
}

bool iso::ScheduledEvent::isSpeedDependant() const {
    return this->data.speed_dependant;
}