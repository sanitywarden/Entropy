#include "menu.hpp"

using namespace iso;

Menu::Menu(SimulationManager* manager) : Gamestate(manager, "menu") {
    this->manager = manager;
    
    this->initialise();
    this->loadResources();
    this->createUI();
}

Menu::~Menu() {

}

void Menu::initialise() {
    auto window_size = this->manager->window.windowSize();

    this->view_game.setSize(window_size);
    this->view_game.setCenter(window_size.x / 2, window_size.y / 2);

    this->view_interface.setSize(window_size);
    this->view_interface.setCenter(window_size.x / 2, window_size.y / 2);

    this->controls.addKeyMappingCheck("key_tilde", sf::Keyboard::Key::Tilde);
}

void Menu::loadResources() {

}

void Menu::handleInput() {
    while(this->manager->window.getWindow()->pollEvent(this->event)) {    
        switch(this->event.type) {
            case sf::Event::Closed: {
                this->manager->exitApplication(0);
                break;
            }

            case sf::Event::KeyPressed: {
                for(const auto& pair : this->controls.key_map) {
                    const auto& name  = pair.first;
                    const int   state = this->controls.isKeyPressed(name);

                    if(this->controls.key_state.count(name))
                        this->controls.key_state[name] = state;
                    else this->controls.key_state.insert({ name, state });
                }

                if(this->controls.keyState("key_tilde")) {
                    this->toggleComponentVisibility("component_debug_performance");
                }
            }

            case sf::Event::KeyReleased: {
                for(const auto& pair : this->controls.key_map) {
                    const auto& name          = pair.first;
                    const int   state_old     = this->controls.key_state[name];
                    const int   state_current = this->controls.isKeyPressed(name);

                    if(!(state_old && state_current))
                        this->controls.key_state[name] = state_current;  
                }
                break;
            }

             case sf::Event::MouseButtonPressed: {
                this->controls.mouse_left   = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
                this->controls.mouse_right  = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
                this->controls.mouse_middle = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
                
                break;
            }

             case sf::Event::MouseButtonReleased: {
                this->controls.mouse_left   = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
                this->controls.mouse_right  = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
                this->controls.mouse_middle = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
                
                break;
            }
        }
    }
}

void Menu::createUI() {
    static gui::DebugPerformance debug_performance(this->manager);
    static gui::WidgetMainMenu widget_main_menu(this->manager);
        widget_main_menu.show = true;

    this->addInterfaceComponent(&debug_performance);
    this->addInterfaceComponent(&widget_main_menu);
}

void Menu::update(float delta_time) {
    this->updateMousePosition();
    this->handleInput();
    this->updateUI();
}

void Menu::render(float delta_time) {
    this->manager->window.clear(COLOUR_WHITE);
    this->manager->window.getWindow()->setView(this->view_interface);

    this->renderUI();

    this->manager->window.display();
}
