#include "worldmap.hpp"

using namespace iso;

Worldmap::Worldmap(entropy::Entropy* engine) {
    this->engine   = engine;
    this->state_id = "Worldmap";

    std::srand(time(0));

    this->initialise();
    this->loadResources();   
    this->createInterface();

    this->world = worldGenerator(this->world_settings, this->region_settings, this->engine);
    this->world.generateWorld();

    this->region_gamestate = Regionmap(this->engine, &this->world);
    this->engine->gamestate.addGamestate("regionmap", this->region_gamestate);
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

    this->debug_text.setFont(this->engine->resource.getFont("garamond"));
    this->debug_text.setFillColor(sf::Color::Black);
    this->debug_text.setCharacterSize(16);
    this->debug_text.setPosition(0, 0);

    this->world_settings.size.x                 = 100;
    this->world_settings.size.y                 = 100;
    this->world_settings.margin.x               = this->world_settings.size.x / 10;
    this->world_settings.margin.y               = this->world_settings.size.y / 10;
    this->world_settings.panel_size.x           = 64;
    this->world_settings.panel_size.y           = 32;
    this->world_settings.noise_octaves          = 16;
    this->world_settings.noise_persistence      = 4;
    this->world_settings.noise_bias             = 4;
    this->world_settings.moisture_octaves       = 8;
    this->world_settings.moisture_persistence   = 8;
    this->world_settings.moisture_bias          = 4;
    this->world_settings.minimum_terrain_height = 0.3f;
    this->world_settings.river_quantity         = (this->world_settings.size.x * this->world_settings.size.y) / ((this->world_settings.size.x * this->world_settings.size.y) / ((this->world_settings.margin.x + this->world_settings.margin.y) / 2));
    this->world_settings.pole_size              = this->world_settings.size.y / 10;
    this->world_settings.multiplier_noise       = 1.25f; // Magic number to make the world look as good as possible.
    this->world_settings.multiplier_gradient    = 2.00f; // Magic number to make the world look as good as possible.
    this->world_settings.multiplier_moisture    = 0.90f; // Magic number to make the world look as good as possible.

    this->region_settings.size.x        = 100;
    this->region_settings.size.y        = 100;
    this->region_settings.tile_offset.x = 100;
    this->region_settings.tile_offset.y = 100;
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

    this->view_interface.setSize(this->engine->window.windowSize());
    this->view_interface.setCenter(this->engine->window.windowWidth() / 2, this->engine->window.windowHeight() / 2);
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
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_tl",  sf::IntRect(0, 32, 64, 32  ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_tr",  sf::IntRect(64, 32, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_bl",  sf::IntRect(0, 64, 64, 32  ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_br",  sf::IntRect(64, 64, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_horizontal", sf::IntRect(128, 32, 64, 32));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_vertical",   sf::IntRect(128, 64, 64, 32));
    this->engine->resource.loadTexture("./res/panels/panel_foliage_atlas.png", "panel_tree_cold",     sf::IntRect(0, 0, 64, 32  ));
    this->engine->resource.loadTexture("./res/panels/panel_foliage_atlas.png", "panel_tree_warm",     sf::IntRect(128, 0, 64, 32));
    this->engine->resource.loadTexture("./res/panels/panel_foliage_atlas.png", "panel_tree_tropical", sf::IntRect(64, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/default.png", "default");

    this->engine->resource.loadFont("./res/font/proggy.ttf",   "proggy");
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
    float x_multiplier = 6.0f;
    float y_multiplier = 6.0f;    

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
    
        this->zoom_camera = false;
    } 
}

void Worldmap::updateCamera() {
    if(this->move_camera) 
        this->moveCamera();

    if(this->zoom_camera) 
        this->zoomCamera();

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

void Worldmap::update(float time_per_frame) {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();
    this->updateInterface();
    this->updateSelectedPanel();
}

void Worldmap::render(float time_per_frame) {
    this->engine->window.clear(sf::Color(50, 50, 50));
    this->engine->window.getWindow()->setView(this->view_game);

    this->renderWorld();
    this->highlightPanel();
    this->drawSelectedPanel();

    this->engine->window.getWindow()->setView(this->view_interface);

    this->drawInterface();

    this->engine->window.getWindow()->setView(this->view_game);
    this->engine->window.display();
}

void Worldmap::handleInput() {
    while(this->engine->window.getWindow()->pollEvent(this->event)) {    
        switch(this->event.type) {
            case sf::Event::Closed: {
                this->engine->exitApplication(0);
                break;
            }

            case sf::Event::MouseButtonPressed: {
                this->mouse_pressed = true;

                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    this->position_pressed = this->mouse_position_window;
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

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))   this->draw_debug = !this->draw_debug;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))      this->world.generateWorld();
                
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

                break;
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

        // Mouse button pressed is used in multiple situations.
        // Distinguish from dragging the screen and selecting a option.
        if(this->mouse_moved && this->mouse_pressed) this->mouse_drag = true;

        else {
            this->mouse_drag = false;
            this->mouse_moved = false;
        }
    }
}

void Worldmap::renderWorld() {
    sf::Vector2f camera_size   = this->view_game.getSize();
    sf::Vector2f camera_centre = this->view_game.getCenter();

    sf::Rect camera_screen_area(camera_centre - 0.5f * camera_size, camera_size);

    for(Region& panel : this->world.world_map) {        
        sf::Rect region_screen_area(panel.position, panel.size);

        // Draw only the regions which you can see on the screen.
        if(camera_screen_area.intersects(region_screen_area))
            this->engine->window.draw(panel);
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
        sf::Vector2i panel_grid_position = sf::Vector2i(
            this->mouse_position_window.x / this->world_settings.panel_size.x,
            this->mouse_position_window.y / this->world_settings.panel_size.y
        );

        const int index = panel_grid_position.y * this->world_settings.size.x + panel_grid_position.x;
        if(this->selected_panel_index != index) 
            this->selected_panel_index = -1;
    }
}

void Worldmap::drawSelectedPanel() {
    if(this->selected_panel_index != -1 && this->selected_panel_index < 0 && this->selected_panel_index > this->world_settings.size.x * this->world_settings.size.y) {
        sf::Vector2f panel_position = this->world.world_map[this->selected_panel_index].position;

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

        this->engine->window.draw(selection_indicator, states);
    }
}

void Worldmap::updateSelectedPanel() {
    if(this->event.type == sf::Event::MouseButtonPressed) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            this->selectPanel();
            this->unselectPanel();
        }
    }
}

void Worldmap::highlightPanel() {
    auto panel_grid_position = sf::Vector2i(
        this->mouse_position_window.x / this->world_settings.panel_size.x,
        this->mouse_position_window.y / this->world_settings.panel_size.y
    );

    auto highlight_position = sf::Vector2f(
        panel_grid_position.x * this->world_settings.panel_size.x,
        panel_grid_position.y * this->world_settings.panel_size.y
    );

    sf::VertexArray highlight(sf::Quads, 4);

    highlight[0].position = highlight_position;
    highlight[1].position = highlight_position + sf::Vector2f(this->world_settings.panel_size.x, 0);
    highlight[2].position = highlight_position + sf::Vector2f(this->world_settings.panel_size.x, this->world_settings.panel_size.y);
    highlight[3].position = highlight_position + sf::Vector2f(0, this->world_settings.panel_size.y);

    highlight[0].texCoords = sf::Vector2f(0, 0);
    highlight[1].texCoords = sf::Vector2f(this->world_settings.panel_size.x, 0);
    highlight[2].texCoords = sf::Vector2f(this->world_settings.panel_size.x, this->world_settings.panel_size.y);
    highlight[3].texCoords = sf::Vector2f(0, this->world_settings.panel_size.y);

    sf::RenderStates states;
    states.texture = &this->engine->resource.getTexture("panel_highlight");

    this->engine->window.draw(highlight, states);
}

void Worldmap::createInterface() {    
    static gui::Button button1;
    button1.setWidgetSize(sf::Vector2f(80, 30));
    button1.setWidgetPosition(sf::Vector2f(5, 10));
    button1.setWidgetColour(sf::Color(50, 80, 130));
    button1.setWidgetID("button_enter_region");
    
    static gui::Widget widget1;
    widget1.setWidgetSize(sf::Vector2f(90, 50));
    widget1.setWidgetPosition(sf::Vector2f(0, 0));
    widget1.setWidgetColour(sf::Color(0, 0, 0, 191));
    widget1.attachComponent(&button1, button1.getWidgetID());
    
    static gui::Label label1(&button1, "Visit region", this->engine->resource.getFont("garamond"), 12);
    label1.align(gui::Alignment::ALIGNMENT_CENTRED);
    button1.setTextComponent(label1);
    
    this->m_interface.insert({ "widget", &widget1 });
}

void Worldmap::updateInterface() {
    std::string debug_text;
    
    auto panel_grid_position = sf::Vector2i(
        this->mouse_position_window.x / this->world.world_settings.panel_size.x,
        this->mouse_position_window.y / this->world.world_settings.panel_size.y
    );

    const int panel_index = panel_grid_position.y * this->world.world_settings.size.x + panel_grid_position.x;
    auto& panel = this->world.world_map[panel_index];

    debug_text += "Frames per second: " + std::to_string(this->engine->getFramesPerSecond()) + "\n";
    debug_text += "Time per frame: "    + std::to_string(this->engine->getTimePerFrame()) + "ms\n";

    debug_text += "Index: " + std::to_string(panel_index) + "\n";
    debug_text += "Biome: " + panel.biome.biome_name + "\n";

    if(this->selected_panel_index != -1)
        debug_text += "\nSelected index: " + std::to_string(this->selected_panel_index) + "\n";

    this->debug_text.setString(debug_text);

    auto* widget = (gui::Widget*)this->m_interface["widget"];
    gui::Button& button1 = *(gui::Button*)(&widget->getComponentByName("button_enter_region"));

    if(button1.containsPoint(this->mouse_position_interface) && this->mouse_pressed) {        
        auto& region = this->world.world_map[this->selected_panel_index];

        // You check for these things here to avoid calling functions responsible for world generation.
        if(region.biome.biome_name == BIOME_ARCTIC.biome_name || region.biome.biome_name == BIOME_OCEAN.biome_name) {
            this->selected_panel_index = -1;
            return;
        }
    
        this->move_camera   = false;
        this->zoom_camera   = false;
        this->mouse_pressed = false;
        this->mouse_drag    = false;
            
        // You have to generate the region first, because setCurrentRegion() depends on it being generated.
        if(!region.visited)
            this->world.generateRegion(this->selected_panel_index, region);

        this->region_gamestate.setCurrentRegion(this->selected_panel_index);
        this->engine->gamestate.setGamestate("regionmap");
    }

    // Rest of the updates.
}

void Worldmap::drawInterface() {
    this->engine->window.draw(this->debug_text);
    
    if(this->selected_panel_index != -1 && this->draw_debug) {
        gui::Widget* widget = (gui::Widget*)this->m_interface["widget"];
        this->engine->window.draw(*widget);
    }
}
