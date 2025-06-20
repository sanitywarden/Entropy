#include "gamestate.hpp"
#include "simulationManager.hpp"
#include "globalutilities.hpp"

#include <filesystem>

namespace iso {
Gamestate::Gamestate(const std::string& state_id) 
    : state_id(state_id), move_camera(false), zoom_camera(false), default_zoom(0), current_zoom(0), max_zoom_in(0), max_zoom_out(0)
{
    this->createUI();
    std::cout << "[] Loaded " << this->interface.size() << " interface page(s) for " << this->state_id << ".\n";
}

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
    
    printError(this->state_id + "::isComponentVisible()", "Interface with id '" + interface_id + "' does not exist");
    return false;
}

void Gamestate::toggleComponentVisibility(const std::string& interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page.get()->toggleVisibile();  

        interface_page.get()->handleGUIEvent("onVisibilityToggle");
        
        auto visibility = interface_page.get()->isVisible();
        visibility == true
            ? interface_page.get()->handleGUIEvent("onShow")
            : interface_page.get()->handleGUIEvent("onHide");
        
        for(auto& component : interface_page.get()->interface)
            component.get()->setVisible(visibility);
        return;
    }

    printError(this->state_id + "::toggleComponentVisibility()", "Interface with id '" + interface_id + "' does not exist");
}

void Gamestate::setVisibilityTrue(const std::string& interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page.get()->setVisible(true);

        interface_page.get()->handleGUIEvent("onShow");
        
        for(auto& component : interface_page.get()->interface)
            component.get()->setVisible(true);
        return;
    }

    printError(this->state_id + "::setVisibilityTrue()", "Interface with id '" + interface_id + "' does not exist");
}

void Gamestate::setVisibilityFalse(const std::string& interface_id) {
    if(this->checkComponentExist(interface_id)) {
        auto interface_page = this->interface.at(interface_id);
        interface_page.get()->setVisible(false);
        
        interface_page.get()->handleGUIEvent("onHide");

        for(auto& component : interface_page.get()->interface)
            component.get()->setVisible(false);
        return;
    }

    printError(this->state_id + "::setVisibilityFalse()", "Interface with id '" + interface_id + "' does not exist");
}

void Gamestate::createUI() {
    namespace fs = std::filesystem;
    
    auto file_exists = [](const std::string& directory) {
        auto path = fs::path(directory);
        return fs::is_directory(path);
    };

    std::string path = "./data/interface/";
    auto directory_name = toLower(this->state_id);
    auto gamestate_dir = path + directory_name + "/";
    auto all_dir = path + "all/"; 

    if(file_exists(gamestate_dir)) {
        for(const auto& file : fs::directory_iterator("./data/interface/" + directory_name + "/")) {
            const auto& filename = readBefore(file.path().string(), ".lua");
            const auto& extension = file.path().extension();

            if(extension == ".luagui") {
                auto interface_page = gui::InterfaceComponent(new gui::InterfacePage(filename + ".luagui", filename + ".luafun"));   
                this->addInterfaceComponent(interface_page);
            }
        }
    }

    if(file_exists(all_dir)) {
        for(const auto& file : fs::directory_iterator("./data/interface/all/")) {
            const auto& filename = readBefore(file.path().string(), ".lua");
            const auto& extension = file.path().extension();
    
            if(extension == ".luagui") {
                auto interface_page = gui::InterfaceComponent(new gui::InterfacePage(filename + ".luagui", filename + ".luafun"));   
                this->addInterfaceComponent(interface_page);
            }
        }
    }
}

void Gamestate::renderUI() const {
    for(const auto& pair : this->interface) {
        auto page = pair.second;
        if(page.get()->isVisible()) {
            page.get()->handleGUIEvent("render");
        }
    }
    
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

void Gamestate::redrawUI() const {
    for(auto& pair : this->interface) {
        auto& interface_page = pair.second;
        
        if(interface_page.get()->isVisible())
            interface_page.get()->handleGUIEvent("update");
    }
}

void Gamestate::updateUI() const {
    for(auto& pair : this->interface) {
        auto& interface_page = pair.second;
        auto visibility = interface_page.get()->isVisible();

        visibility == true
            ? interface_page.get()->checkShouldClose()
            : interface_page.get()->checkShouldOpen();
        
        // Trigger update function
        if(interface_page.get()->isVisible())
            interface_page.get()->handleGUIEvent("update");
    }
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
    auto window_size = game_manager.window.getWindowSize().asSFMLVector2f();
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