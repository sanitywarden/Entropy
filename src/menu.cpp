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

    this->controls.addKeyMappingCheck("key_tilde", sf::Keyboard::Key::Tilde);
}

void Menu::loadResources() {
    this->manager->resource.loadTexture("./res/ui/background.png", "background", sf::IntRect(0, 0, 2560, 1440));

    auto texture_size = this->manager->resource.getTextureSize("background");
    this->view_game.setSize(texture_size);
    this->view_game.setCenter(texture_size.x / 2, texture_size.y / 2);

    this->view_interface.setSize(texture_size);
    this->view_interface.setCenter(texture_size.x / 2, texture_size.y / 2);
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

            case sf::Event::Resized: {
                auto new_window_size = sf::Vector2f(
                    this->event.size.width,
                    this->event.size.height
                );
            
                auto texture_size = this->manager->resource.getTextureSize("background");

                this->view_game.setCenter(texture_size.x / 2, texture_size.y / 2);
                this->view_interface.setCenter(texture_size.x / 2, texture_size.y / 2);
                this->manager->font_size = (this->manager->window.windowWidth() + this->manager->window.windowHeight()) / 160;

                this->manager->window.getWindow()->setView(this->view_interface);
                this->resizeViews();
                this->resizeUI();
                this->manager->window.getWindow()->setView(this->view_game);

                break; 
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

    this->renderBackground();
    this->renderUI();

    this->manager->window.display();
}

void Menu::renderBackground() {
    sf::VertexArray background(sf::Quads, 4);
    
    auto texture_size = this->manager->resource.getTextureSize("background");

    background[0].position = sf::Vector2f(0, 0); 
    background[1].position = sf::Vector2f(texture_size.x, 0); 
    background[2].position = sf::Vector2f(texture_size.x, texture_size.y); 
    background[3].position = sf::Vector2f(0, texture_size.y);

    background[0].texCoords = sf::Vector2f(0, 0); 
    background[1].texCoords = sf::Vector2f(texture_size.x, 0);  
    background[2].texCoords = sf::Vector2f(texture_size.x, texture_size.y); 
    background[3].texCoords = sf::Vector2f(0, texture_size.y); 

    sf::RenderStates states;
    states.texture = &this->manager->resource.getTexture("background");
    this->manager->window.draw(background, states);
}

void Menu::resizeViews() {
    auto background_size = this->manager->resource.getTextureSize("background");
    this->view_game.setSize(background_size);
    this->view_interface.setSize(background_size);
}