#include "gamestate.hpp"

#include <iostream>

using namespace entropy;

Gamestate::Gamestate() {
    this->engine   = nullptr;
    this->state_id = "default";

    this->move_camera = false;
    this->zoom_camera = false;

    this->default_zoom = 0;
    this->current_zoom = 0;
    this->max_zoom_in  = 0;
    this->max_zoom_out = 0;
}

Gamestate::Gamestate(Entropy* engine, std::string state_id) {
    this->engine   = engine;
    this->state_id = state_id;
}

Gamestate::~Gamestate() {
    
}

void Gamestate::updateMousePosition() {
    this->engine->window.getWindow()->setView(this->view_game);
    this->mouse_position_desktop = sf::Mouse::getPosition(*this->engine->window.getWindow());
    this->mouse_position_window  = this->engine->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    
    this->engine->window.getWindow()->setView(this->view_interface);
    this->mouse_position_interface = this->engine->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    this->engine->window.getWindow()->setView(this->view_game);
}

int Gamestate::getDefaultZoom() const {
    return this->default_zoom;
}

int Gamestate::getMaxZoomIn() const {
    return this->max_zoom_in;
} 

int Gamestate::getMaxZoomOut() const {
    return this->max_zoom_out;
}

bool Gamestate::shouldCameraMove() const {
    return this->move_camera;
} 

bool Gamestate::shouldCameraZoom() const {
    return this->zoom_camera;
}

void Gamestate::addInterfaceComponent(gui::InterfacePage* interface_component) {
    std::string id = interface_component->getWidgetID();
    this->interface[id] = interface_component;
}

gui::InterfacePage* Gamestate::getInterfaceComponent(std::string interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        return interface_page;
    }

    return nullptr;
}

bool Gamestate::checkComponentExist(std::string interface_id) {
    return this->interface.count(interface_id);
}

void Gamestate::toggleComponentVisibility(std::string interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page->show = !interface_page->show;  
    }
}

void Gamestate::setVisibilityTrue(std::string interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page->show = true;
    }
}

void Gamestate::setVisibilityFalse(std::string interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page->show = false;
    }
}

void Gamestate::renderUI() const {
    for(const auto& pair : this->interface) {
        auto* component = pair.second;
        if(component)
            if(component->isVisible())
                this->engine->window.draw(*component);
    }
}

void Gamestate::updateUI() const {
    for(const auto& pair : this->interface) {
        auto* component = pair.second;
        
        if(component) {
            if(component->isVisible()) {
                component->updateUI();
                component->functionality();
            }
        }
    }
}

void Gamestate::resizeUI() const {
    for(const auto& pair : this->interface) {
        auto* component = pair.second;

        if(component) {
            component->createUI();
        }
    }
}

bool Gamestate::mouseIntersectsUI() const {
    for(const auto& pair : this->interface) {
        const auto* component = pair.second;
        
        if(component)
            if(component->intersectsUI(this->mouse_position_interface) && component->isVisible())
                return true;
    }
    
    return false;
}

bool Gamestate::pointIntersectsUI(sf::Vector2f point) const {
    for(const auto& pair : this->interface) {
        const auto* component = pair.second;
        
        if(component)
            if(component->intersectsUI(point) && component->isVisible())
                return true;
    }
    
    return false;
}