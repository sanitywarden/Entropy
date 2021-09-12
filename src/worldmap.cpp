#include "worldmap.hpp"

using namespace iso;

Worldmap::Worldmap(entropy::Entropy* engine) {
    this->engine   = engine;
    this->state_id = "Worldmap";

    std::srand(time(0));

    this->initialise();
    this->loadResources();   

    this->world = worldGenerator(this->world_settings, this->region_settings, this->engine);
    this->world.generateWorld();

    this->region_gamestate = Regionmap(this->engine, &this->world);
}        

Worldmap::~Worldmap() {

}

void Worldmap::initialise() {
    this->mouse_pressed  = false;
    this->mouse_moved    = false;
    this->mouse_drag     = false;
    this->mouse_wheel_up = false;
    this->move_camera    = false;
    this->zoom_camera    = false;

    this->selected_panel_index = -1;
    this->draw_debug = true;

    this->zoom = 0;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 3;

    this->widget_text.setFont(this->engine->resource.getFont("garamond"));
    this->widget_text.setFillColor(sf::Color::Black);
    this->widget_text.setCharacterSize(16);

    this->debug_text.setFont(this->engine->resource.getFont("garamond"));
    this->debug_text.setFillColor(sf::Color::Black);
    this->debug_text.setCharacterSize(16);

    this->world_settings.size.x               = 100;
    this->world_settings.size.y               = 100;
    this->world_settings.margin.x             = this->world_settings.size.x / 10;
    this->world_settings.margin.y             = this->world_settings.size.y / 10;
    this->world_settings.panel_size.x         = 64;
    this->world_settings.panel_size.y         = 32;
    this->world_settings.noise_octaves        = 16;
    this->world_settings.noise_persistence    = 4;
    this->world_settings.noise_bias           = 4;
    this->world_settings.moisture_octaves     = 8;
    this->world_settings.moisture_persistence = 8;
    this->world_settings.moisture_bias        = 4;
    this->world_settings.river_quantity       = (this->world_settings.size.x * this->world_settings.size.y) / ((this->world_settings.size.x * this->world_settings.size.y) / ((this->world_settings.margin.x + this->world_settings.margin.y) / 2));
    this->world_settings.pole_size            = this->world_settings.size.y / 10;
    this->world_settings.multiplier_noise     = 1.25f; // Magic number to make the world look as good as possible.
    this->world_settings.multiplier_gradient  = 2.00f; // Magic number to make the world look as good as possible.
    this->world_settings.multiplier_moisture  = 0.90f; // Magic number to make the world look as good as possible.

    this->region_settings.size.x        = 40;
    this->region_settings.size.y        = 40;
    this->region_settings.tile_offset.x = 0;
    this->region_settings.tile_offset.y = 0;
    this->region_settings.tile_size.x   = 64;
    this->region_settings.tile_size.y   = 32;

    this->view_game.setCenter(sf::Vector2f(
        this->world_settings.panel_size.x * this->world_settings.size.x / 2,
        this->world_settings.panel_size.y * this->world_settings.size.y / 2
    ));

    this->view_game.setSize(sf::Vector2f(
        this->world_settings.panel_size.x * this->world_settings.size.x / (this->world_settings.size.x / 4),
        this->world_settings.panel_size.y * this->world_settings.size.y / (this->world_settings.size.y / 4)
    ));

    auto window_size = this->engine->window.getWindowSize();

    this->view_interface.setSize(window_size);
    this->view_interface.setCenter(window_size.x / 2, window_size.y / 2);

    this->createDebugTab();
    this->createPanelTab();
}

void Worldmap::loadResources() {
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_highlight",        sf::IntRect(0, 288, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_cold",       sf::IntRect(0, 0, 64, 32   ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_warm",       sf::IntRect(64, 0, 64, 32  ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_tropical",   sf::IntRect(128, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_subtropical",sf::IntRect(512, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_ocean",            sf::IntRect(192, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_sea",              sf::IntRect(256, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_arctic",           sf::IntRect(320, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_tundra",           sf::IntRect(384, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_desert",           sf::IntRect(448, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_savannah",         sf::IntRect(576, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_tl",  sf::IntRect(0, 32, 64, 32  ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_tr",  sf::IntRect(64, 32, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_bl",  sf::IntRect(0, 64, 64, 32  ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_br",  sf::IntRect(64, 64, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_horizontal", sf::IntRect(128, 32, 64, 32));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_vertical",   sf::IntRect(128, 64, 64, 32));

    this->engine->resource.loadFont("./res/font/proggy.ttf", "proggy");
    this->engine->resource.loadFont("./res/font/garamond.ttf", "garamond");
}

void Worldmap::updateMousePosition() {
    this->mouse_position_desktop = sf::Mouse::getPosition(*this->engine->window.getWindow());
    this->mouse_position_window  = this->engine->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    
    this->engine->window.getWindow()->setView(this->view_interface);
    this->mouse_position_interface = this->engine->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    this->engine->window.getWindow()->setView(this->view_game);
}

void Worldmap::moveCamera() {
    auto distance = sf::Vector2f(
        (this->position_pressed.x - this->position_released.x) / this->world_settings.panel_size.x / 2,       
        (this->position_pressed.y - this->position_released.y) / this->world_settings.panel_size.y / 2
    );

    // Multipliers for faster camera movement. 
    float x_multiplier = 3.0f;
    float y_multiplier = 3.0f;    

    // Check the horizontal and vertical bounds of the screen.
    // This makes sure that you can not move past the world map.
    if(this->view_game.getCenter().x + (distance.x * x_multiplier) < (this->world_settings.size.x * this->world_settings.panel_size.x) - this->view_game.getSize().x / 2.f && this->view_game.getCenter().x + (distance.x * x_multiplier) > 0.f + this->view_game.getSize().x / 2.f) {
        this->view_game.move(x_multiplier * distance.x, 0.f);
    }

    if(this->view_game.getCenter().y + (distance.y * y_multiplier) < (this->world_settings.size.y * this->world_settings.panel_size.y) - this->view_game.getSize().y / 2.f && this->view_game.getCenter().y + (distance.y * y_multiplier) > 0.f + this->view_game.getSize().y / 2.f) {
        this->view_game.move(0.f, y_multiplier * distance.y);
    }

    this->move_camera = false;
}

void Worldmap::zoomCamera() {
    // If you scroll up   - zoom in.
    // If you scroll down - zoom out.

    if((this->zoom > this->max_zoom_in && this->mouse_wheel_up) || (this->zoom < this->max_zoom_out && !this->mouse_wheel_up)) {  
        if(this->mouse_wheel_up) {
            this->zoom = this->zoom - 1;

            this->view_game.zoom(0.5f);
        }
        
        else if(!this->mouse_wheel_up) {
            this->zoom = this->zoom + 1;

            this->view_game.zoom(2.f);
        }
    } 
}

void Worldmap::updateCamera() {
    if(this->move_camera) {
        this->moveCamera();
        this->move_camera   = false;
    }

    if(this->zoom_camera) {
        this->zoomCamera();
        this->zoom_camera = false;
    }

    // Check if the camera is postioned badly on the top of the screen.
    if(this->view_game.getCenter().y - this->view_game.getSize().y / 2 < 0) {
        float offset_y = std::abs(this->view_game.getCenter().y - this->view_game.getSize().y / 2);

        this->view_game.move(0.f, offset_y);
    }

    // Check if the camera is postioned badly on the left of the screen.
    if(this->view_game.getCenter().x - this->view_game.getSize().x / 2 < 0) {
        float offset_x = std::abs(this->view_game.getCenter().x - this->view_game.getSize().x / 2);

        this->view_game.move(offset_x, 0.f);
    }

    // Check if the camera is postioned badly on the right of the screen.
    if(this->view_game.getCenter().x + this->view_game.getSize().x / 2 > this->world_settings.panel_size.x * this->world_settings.size.x) {
        float offset_x = -(this->view_game.getCenter().x + this->view_game.getSize().x / 2);

        this->view_game.move(offset_x, 0.f);
    } 
    
    // Check if the camera is positioned badly on the bottom of the screen.
    if(this->view_game.getCenter().y + this->view_game.getSize().y / 2 > this->world_settings.panel_size.y * this->world_settings.size.y) {
        float offset_y = -(this->view_game.getCenter().y + this->view_game.getSize().y / 2);

        this->view_game.move(0.f, offset_y);
    }
}

void Worldmap::update() {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();
    this->updateDebugTab();
    this->updatePanelTab();
    this->updateSelectedPanel();
}

void Worldmap::render() {
    this->engine->window.clear(sf::Color(50, 50, 50));
    this->engine->window.getWindow()->setView(this->view_game);

    this->renderWorld();
    this->highlightPanel();
    this->drawSelectedPanel();

    this->engine->window.getWindow()->setView(this->view_interface);

    this->renderDebugTab();
    this->renderPanelTab(); 

    this->engine->window.getWindow()->setView(this->view_game);
    this->engine->window.display();
}

void Worldmap::handleInput() {
    while(this->engine->window.getWindow()->pollEvent(this->event)) {    
        switch(this->event.type) {
            case sf::Event::Closed: {
                entropy::Entropy::quitApplication(0);
                break;
            }

            case sf::Event::MouseButtonPressed: {
                this->mouse_pressed = true;

                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    this->position_pressed = this->mouse_position_window;

                    this->selectPanel();
                    this->unselectPanel();
                }

                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                    if(this->selected_panel_index != -1) {
                        this->selected_panel_index = -1;
                    }
                }

                break;
            }

            case sf::Event::MouseButtonReleased: {
                this->mouse_pressed = false;

                break;
            }

            case sf::Event::KeyPressed: {
                const float modifier = 2.0f;

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) this->draw_debug = !this->draw_debug;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))    this->world.generateWorld();

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                    this->view_game.move(0, -this->world_settings.panel_size.y * modifier);

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    this->view_game.move(-this->world_settings.panel_size.x * modifier, 0);

                if(this->view_game.getCenter().y + (this->world_settings.panel_size.y * modifier) <= (this->world_settings.size.y * this->world_settings.panel_size.y) - this->view_game.getSize().y / 2)
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                        this->view_game.move(0, this->world_settings.panel_size.y * modifier);

                if(this->view_game.getCenter().x + (this->world_settings.panel_size.x * modifier) <= (this->world_settings.size.x * this->world_settings.panel_size.x) - this->view_game.getSize().x / 2)
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                        this->view_game.move(this->world_settings.panel_size.x * modifier, 0);
            }

            case sf::Event::MouseMoved: {
                this->mouse_moved = true;

                if(!this->mouse_drag) break;

                this->position_released = this->mouse_position_window;

                // Avoid accidental drags.
                // This also helps to record button presses (without the intention of dragging the screen).
                if(this->mouse_drag && std::abs(this->position_pressed.x - this->position_released.x) > 10 && std::abs(this->position_pressed.y - this->position_released.y) > 10) this->move_camera = true;

                break;
            }

            case sf::Event::MouseWheelScrolled: {          
                if(this->event.mouseWheelScroll.delta == -1) this->mouse_wheel_up = false; 
                if(this->event.mouseWheelScroll.delta == 1)  this->mouse_wheel_up = true; 
                
                this->zoom_camera = true;
                
                break;
            }
        }
    }

    // Mouse button pressed is used in multiple situations.
    // Distinguish from dragging the screen and selecting a option.
    if(this->mouse_moved && this->mouse_pressed) this->mouse_drag = true;

    else {
        this->mouse_drag = false;
        this->mouse_moved = false;
    }
}

void Worldmap::renderWorld() {
    for(auto& panel : this->world.world_map) {
        this->engine->window.getWindow()->draw(panel);
    }
}

void Worldmap::selectPanel() {
    if(this->mouse_pressed && !this->mouse_moved && !this->mouse_drag) {
        sf::Vector2i panel_grid_position = sf::Vector2i(
            this->mouse_position_window.x / this->world_settings.panel_size.x,
            this->mouse_position_window.y / this->world_settings.panel_size.y
        );

        const int index = panel_grid_position.y * this->world_settings.size.x + panel_grid_position.x;

        this->selected_panel_index = index;
    }
}

void Worldmap::unselectPanel() {
    if(this->mouse_pressed && !this->mouse_moved && !this->mouse_drag && this->selected_panel_index != -1) {
        auto panel_grid_position = sf::Vector2i(
            this->mouse_position_window.x / this->world_settings.panel_size.x,
            this->mouse_position_window.y / this->world_settings.panel_size.y
        );

        const int index = panel_grid_position.y * this->world_settings.size.x + panel_grid_position.x;

        if(this->selected_panel_index != index) 
            this->selected_panel_index = -1.0;
    }
}

void Worldmap::drawSelectedPanel() {
    if(this->selected_panel_index != -1) {
        auto panel_position = this->world.world_map[this->selected_panel_index].panel_position;

        sf::VertexArray selection_indicator(sf::Quads, 4);

        selection_indicator[0].position = panel_position;
        selection_indicator[1].position = panel_position + sf::Vector2f(this->world_settings.panel_size.x, 0);
        selection_indicator[2].position = panel_position + sf::Vector2f(this->world_settings.panel_size.x, this->world_settings.panel_size.y);
        selection_indicator[3].position = panel_position + sf::Vector2f(0, this->world_settings.panel_size.y);
    
        selection_indicator[0].texCoords = sf::Vector2f(0, 0);
        selection_indicator[1].texCoords = sf::Vector2f(this->world_settings.panel_size.x, 0);
        selection_indicator[2].texCoords = sf::Vector2f(this->world_settings.panel_size.x, this->world_settings.panel_size.y);
        selection_indicator[3].texCoords = sf::Vector2f(0, this->world_settings.panel_size.y);

        sf::RenderStates states;
        states.texture = &this->engine->resource.getTexture("panel_highlight");

        this->engine->window.getWindow()->draw(selection_indicator, states);
    }
}

void Worldmap::updateSelectedPanel() {

}

void Worldmap::highlightPanel() {
    auto grid_position = sf::Vector2i(
        this->mouse_position_window.x / this->world_settings.panel_size.x,
        this->mouse_position_window.y / this->world_settings.panel_size.y
    );

    auto position = sf::Vector2f(
        grid_position.x * this->world_settings.panel_size.x,
        grid_position.y * this->world_settings.panel_size.y
    );

    sf::VertexArray highlight(sf::Quads, 4);

    highlight[0].position = position;
    highlight[1].position = position + sf::Vector2f(this->world_settings.panel_size.x, 0);
    highlight[2].position = position + sf::Vector2f(this->world_settings.panel_size.x, this->world_settings.panel_size.y);
    highlight[3].position = position + sf::Vector2f(0, this->world_settings.panel_size.y);

    highlight[0].texCoords = sf::Vector2f(0, 0);
    highlight[1].texCoords = sf::Vector2f(this->world_settings.panel_size.x, 0);
    highlight[2].texCoords = sf::Vector2f(this->world_settings.panel_size.x, this->world_settings.panel_size.y);
    highlight[3].texCoords = sf::Vector2f(0, this->world_settings.panel_size.y);

    sf::RenderStates states;
    states.texture = &this->engine->resource.getTexture("panel_highlight");

    this->engine->window.getWindow()->draw(highlight, states);
}

void Worldmap::createDebugTab() {

}

void Worldmap::updateDebugTab() {
    std::string widget_string;

    auto grid_position = sf::Vector2i(
        this->mouse_position_window.x / this->world_settings.panel_size.x,
        this->mouse_position_window.y / this->world_settings.panel_size.y
    );

    const int index = grid_position.y * this->world_settings.panel_size.x + grid_position.x;

    widget_string += std::to_string(this->engine->fps.get()) + " FPS\n";

    widget_string += "Noise: "       + std::to_string(this->world.world_map[index].region.height)      + "\n";
    widget_string += "Latitude: "    + std::to_string(this->world.world_map[index].region.latitude)    + "\n"; 
    widget_string += "Moisture: "    + std::to_string(this->world.world_map[index].region.moisture)    + "\n";
    widget_string += "Temperature: " + std::to_string(this->world.world_map[index].region.temperature) + "\n";

    std::string terrain = (this->world.world_map[index].is_terrain) ? "True" : "False";
    widget_string += "Terrain: " + terrain + "\n";

    std::string arctic = (this->world.world_map[index].is_arctic) ? "True" : "False";
    widget_string += "Arctic:  " + arctic + "\n";

    std::string river = (this->world.world_map[index].is_river) ? "True" : "False";
    widget_string += "River:   " + river + "\n";

    this->debug_text.setString(widget_string);
    this->debug_text.setPosition(0, 0);
}

void Worldmap::renderDebugTab() {
    if(this->draw_debug) {
        this->engine->window.getWindow()->draw(this->debug_text);
    }
}

void Worldmap::createPanelTab() {
    gui::Button button;
    button.widget_size     = sf::Vector2f(80, 50);
    button.widget_position = sf::Vector2f(0, this->engine->window.getWindowSize().y - button.widget_size.y);
    button.colour          = sf::Color(50, 80, 130);
    
    sf::Text text;
    text.setString("Press me!");
    text.setPosition(0, this->engine->window.getWindowSize().y - button.widget_size.y);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Black);
    text.setFont(this->engine->resource.getFont("garamond"));
    button.setTextComponent(text);

    this->m_buttons.resize(1);
    this->m_buttons[0] = button;
}

void Worldmap::updatePanelTab() {
    std::string widget_string;

    const int index = this->selected_panel_index; 

    if(index == -1 || index < 0 || index > this->world_settings.size.x * this->world_settings.size.y - 1)
        return;

    widget_string += "Selected panel: " + std::to_string(index) + "\n";

    auto& button = this->m_buttons[0];
    
    // If this is true it means that the user wants to view the region.
    if(button.containsPoint(this->mouse_position_interface) && this->mouse_pressed) {
        auto region = Region(BiomeType::GRASSLAND_WARM, 0.5f, 0.5f, 0.5f, 0.5f, this->region_settings.size); 

        this->mouse_pressed = false;

        this->world.generateRegion(region, index);
        this->region_gamestate.setCurrentRegion(index);

        this->engine->gamestate.setGamestate("regionmap");
    }

    this->widget_text.setString(widget_string);
    this->widget_text.setPosition(0, 200);
}

void Worldmap::renderPanelTab() {
    if(this->draw_debug && this->selected_panel_index != -1) {
        this->engine->window.getWindow()->draw(this->widget_text);
        this->engine->window.getWindow()->draw(this->m_buttons[0]);
    }
}