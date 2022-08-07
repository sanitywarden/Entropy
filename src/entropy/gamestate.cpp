#include "gamestate.hpp"

#include <iostream>

using namespace iso;

Gamestate::Gamestate() {
    this->engine   = nullptr;
    this->state_id = "default";

    this->move_camera = false;
    this->zoom_camera = false;

    this->block_keybinds = false;

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

void Gamestate::addInterfaceComponent(gui::InterfaceComponent interface_component) {
    std::string id = interface_component.get()->getWidgetID();
    this->interface[id] = interface_component;
}

gui::InterfacePage* Gamestate::getInterfaceComponent(std::string interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        return interface_page.get();
    }

    return nullptr;
}

bool Gamestate::checkComponentExist(std::string interface_id) const {
    return this->interface.count(interface_id);
}

bool Gamestate::isComponentVisible(std::string interface_id) const {
    if(this->checkComponentExist(interface_id)) {
        auto component = this->interface.at(interface_id);
        return component.get()->isVisible();
    }
    
    return false;
}

void Gamestate::toggleComponentVisibility(std::string interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page.get()->show = !interface_page.get()->show;  
    }
}

void Gamestate::setVisibilityTrue(std::string interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page.get()->show = true;
    }
}

void Gamestate::setVisibilityFalse(std::string interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page.get()->show = false;
    }
}

void Gamestate::renderUI() const {
    // Pages with lower priority end up first in the vector.
    const auto compare = [](
        const std::pair<std::string, std::shared_ptr<gui::InterfacePage>> pair1, 
        const std::pair<std::string, std::shared_ptr<gui::InterfacePage>> pair2
    ) {
        auto page1 = pair1.second;
        auto page2 = pair2.second;

        return page1.get()->getDrawingPriority() < page2.get()->getDrawingPriority(); 
    };

    std::vector <std::pair<std::string, std::shared_ptr<gui::InterfacePage>>> copied_interface;
    if(!copied_interface.size())
        for(auto pair : this->interface)
            copied_interface.push_back(pair);

    // Sort the vector. Pages with low priority are first. Pages with high priority are last.
    // Pages with low priotity are drawn first.
    std::sort(copied_interface.begin(), copied_interface.end(), compare);
    
    // Draw the pages in the correct order.
    for(const auto& pair : copied_interface) {
        auto component = pair.second;
        if(component)
            if(component.get()->isVisible())
                this->engine->window.draw(*component.get());
    }
}

void Gamestate::updateUI() {
    this->view_interface.setCenter(
        this->view_interface.getSize().x / 2,
        this->view_interface.getSize().y / 2
    );

    for(const auto& pair : this->interface) {
        auto component = pair.second;
        
        if(component) {
            if(component.get()->isVisible()) {
                component.get()->updateUI();
                component.get()->functionality();
            }
        }
    }
}

void Gamestate::resizeUI() const {
    for(const auto& pair : this->interface) {
        auto component = pair.second;
        component.get()->createUI();
    }
}

bool Gamestate::mouseIntersectsUI() const {
    for(const auto& pair : this->interface) {
        auto component = pair.second;
        
        if(component.get()->intersectsUI(this->mouse_position_interface) && component.get()->isVisible())
            return true;
    }
    
    return false;
}

bool Gamestate::pointIntersectsUI(sf::Vector2f point) const {
    for(const auto& pair : this->interface) {
        auto component = pair.second;
        
        if(component.get()->intersectsUI(point) && component.get()->isVisible())
            return true;
    }
    
    return false;
}

bool Gamestate::mouseIntersectsUI(const std::string& component_name) const {
    return this->pointIntersectsUI(this->mouse_position_interface, component_name);
}

bool Gamestate::pointIntersectsUI(sf::Vector2f point, const std::string& component_name) const {
    if(this->checkComponentExist(component_name)) {
        auto* component = this->getInterfaceComponent(component_name);
        return component->intersectsUI(point);
    }

    return false;
}

void Gamestate::resizeViews() {
    auto window_size = this->engine->window.windowSize();
    auto scaled_size = sf::Vector2f(
        window_size.x * (this->current_zoom + 1),
        window_size.y * (this->current_zoom + 1)
    );

    this->view_game.setSize(scaled_size);
    
    this->view_interface.setSize(window_size);
    this->view_interface.setCenter(window_size.x / 2, window_size.y / 2);
}