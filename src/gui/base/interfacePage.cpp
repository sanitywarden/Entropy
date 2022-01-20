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
        ? this->interface.at(id)
        : nullptr;
}

bool InterfacePage::intersectsUI(sf::Vector2f point) const {
    for(const auto& pair : this->interface) {
        auto* component = pair.second;

        if(component->containsPoint(point))
            return true;
    }

    return false;
}