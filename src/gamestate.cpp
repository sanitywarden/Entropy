#include "gamestate.hpp"
#include "simulationManager.hpp"
#include "globalutilities.hpp"

namespace iso {
Gamestate::Gamestate(const std::string& state_id) 
    : state_id(state_id), move_camera(false), zoom_camera(false), default_zoom(0), current_zoom(0), max_zoom_in(0), max_zoom_out(0)
{}

Gamestate::~Gamestate() 
{}

void Gamestate::updateMousePosition() {
    game_manager.window.setView(this->view_game);

    auto position_desktop = sf::Mouse::getPosition(*game_manager.window.getWindow());
    auto position_window  = game_manager.window.getWindow()->mapPixelToCoords(position_desktop);

    this->mouse_position_desktop = core::Vector2i(position_desktop.x, position_desktop.y);
    this->mouse_position_window  = core::Vector2i(position_window.x, position_window.y);
    
    game_manager.window.setView(this->view_interface);

    auto position_interface = game_manager.window.getWindow()->mapPixelToCoords(position_desktop);
    this->mouse_position_interface = core::Vector2i(position_interface.x, position_interface.y);
    
    game_manager.window.setView(this->view_game);
}

const std::string& Gamestate::getStateId() const {
    return this->state_id;
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

gui::InterfacePage* const Gamestate::getInterfaceComponent(const std::string& interface_id) const {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        return interface_page.get();
    }

    return nullptr;
}

bool Gamestate::checkComponentExist(const std::string& interface_id) const {
    return this->interface.count(interface_id);
}

bool Gamestate::isComponentVisible(const std::string& interface_id) const {
    if(this->checkComponentExist(interface_id)) {
        auto component = this->interface.at(interface_id);
        return component.get()->isVisible();
    }
    
    printError("Gamestate::isComponentVisible()", "Interface with id '" + interface_id + "' does not exist");
    return false;
}

void Gamestate::toggleComponentVisibility(const std::string& interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page.get()->toggleVisibile();  
        return;
    }

    printError("Gamestate::toggleComponentVisibility()", "Interface with id '" + interface_id + "' does not exist");
}

void Gamestate::setVisibilityTrue(const std::string& interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page.get()->setVisible(true);
        return;
    }

    printError("Gamestate::setVisibilityTrue()", "Interface with id '" + interface_id + "' does not exist");
}

void Gamestate::setVisibilityFalse(const std::string& interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page.get()->setVisible(false);
        return;
    }

    printError("Gamestate::setVisibilityFalse()", "Interface with id '" + interface_id + "' does not exist");
}

void Gamestate::renderUI() const {
    // Pages with lower priority end up first in the vector.
    const auto compare = [](
        const std::pair<std::string, std::shared_ptr<gui::InterfacePage>> pair1, 
        const std::pair<std::string, std::shared_ptr<gui::InterfacePage>> pair2
    ) {
        auto page1 = pair1.second;
        auto page2 = pair2.second;

        return page1.get()->getDrawPriority() < page2.get()->getDrawPriority(); 
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
                game_manager.window.draw(*component.get());
    }
}

void Gamestate::updateUI() {

}   

void Gamestate::resizeUI() {
    this->createUI();
}

bool Gamestate::mouseIntersectsUI() const {
    for(const auto& pair : this->interface) {
        auto component = pair.second;
        
        if(component.get()->intersectsUI(this->mouse_position_interface) && component.get()->isVisible())
            return true;
    }
    
    return false;
}

bool Gamestate::pointIntersectsUI(core::Vector2i point) const {
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

bool Gamestate::pointIntersectsUI(core::Vector2i point, const std::string& component_name) const {
    if(this->checkComponentExist(component_name)) {
        auto component = this->getInterfaceComponent(component_name);
        return component->intersectsUI(point);
    }

    return false;
}

void Gamestate::resizeViews() {
    auto window_size = game_manager.window.getWindowSize();
    auto scaled_size = sf::Vector2f(
        window_size.x * (this->current_zoom + 1),
        window_size.y * (this->current_zoom + 1)
    );

    this->view_game.setSize(scaled_size);
    
    this->view_interface.setSize(window_size);
    this->view_interface.setCenter(window_size.x / 2, window_size.y / 2);
}

void Gamestate::runGUIEventHandle(const std::string& event_name) {
    for(const auto& pair : this->interface) {
        auto component = pair.second.get();
        if(component->isVisible()) {
            component->handleGUIEvent(event_name);
        }
    }
}
}