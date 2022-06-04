#include "interfacePage.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

InterfacePage::InterfacePage() {
    this->manager = nullptr;
    std::cout << "[GUI][InterfacePage]: GUI page manager is a nullptr.\n";
}

InterfacePage::InterfacePage(SimulationManager* manager) {
    this->manager = manager;
    this->show    = false;
}

InterfacePage::~InterfacePage() {

}

AbstractWidget* InterfacePage::getComponent(std::string id) const {
    return this->interface.count(id)
        ? this->interface.at(id).get()
        : nullptr;
}

void InterfacePage::addComponent(std::shared_ptr <AbstractWidget> component) {
    if(component == nullptr)
        return;

    auto id = component.get()->getWidgetID();
    this->interface[id] = component;
}

void InterfacePage::deleteComponent(std::string id) {
    this->interface.erase(id);
}

bool InterfacePage::intersectsUI(sf::Vector2f point) const {
    for(const auto& pair : this->interface) {
        auto* component = pair.second.get();

        if(component->containsPoint(point))
            return true;
    }

    return false;
}
