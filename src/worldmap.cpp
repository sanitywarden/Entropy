#include "worldmap.hpp"

using namespace iso;

Worldmap::Worldmap(SimulationManager* manager) {
    this->manager  = manager;
    this->state_id = "Worldmap";

    std::srand(time(0));

    this->initialise();
    this->loadResources();   
    this->createUI();
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

    this->current_index  = -1;
    this->selected_index = -1;
    this->draw_calls    = 0;

    this->draw_control_panel = false;

    this->zoom = 0;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 3;

    this->view_game.setCenter(sf::Vector2f(
        this->manager->settings.world_panel_size.x * this->manager->settings.world_size / 2,
        this->manager->settings.world_panel_size.y * this->manager->settings.world_size / 2
    ));

    this->view_game.setSize(sf::Vector2f(
        this->manager->settings.world_panel_size.x * this->manager->settings.world_size / (this->manager->settings.world_size / 4),
        this->manager->settings.world_panel_size.y * this->manager->settings.world_size / (this->manager->settings.world_size / 4)
    ));

    this->view_interface.setSize(this->manager->window.windowSize());
    this->view_interface.setCenter(this->manager->window.windowWidth() / 2, this->manager->window.windowHeight() / 2);

    this->controls.addKeyMappingCheck("key_open_control_panel", sf::Keyboard::Key::Escape);
}

void Worldmap::loadResources() {
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_highlight",             sf::IntRect(0, 288, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_cold",            sf::IntRect(0, 0, 64, 32   ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_warm",            sf::IntRect(64, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_tropical",        sf::IntRect(128, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_subtropical",     sf::IntRect(512, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_ocean",                 sf::IntRect(192, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_sea",                   sf::IntRect(256, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_arctic",                sf::IntRect(320, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_tundra",                sf::IntRect(384, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_desert",                sf::IntRect(448, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_tl",       sf::IntRect(0, 32, 64, 32  ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_tr",       sf::IntRect(64, 32, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_bl",       sf::IntRect(0, 64, 64, 32  ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_br",       sf::IntRect(64, 64, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_horizontal",      sf::IntRect(128, 32, 64, 32));
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_vertical",        sf::IntRect(128, 64, 64, 32));
    this->manager->resource.loadTexture("./res/panels/panel_foliage_atlas.png", "panel_tree_cold",     sf::IntRect(0, 0, 64, 32   ));
    this->manager->resource.loadTexture("./res/panels/panel_foliage_atlas.png", "panel_tree_warm",     sf::IntRect(128, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/panels/panel_foliage_atlas.png", "panel_tree_tropical", sf::IntRect(64, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/default.png", "default");

    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_top_left",                sf::IntRect(0, 0, 64, 64    ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_top_right",               sf::IntRect(128, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_bottom_left",             sf::IntRect(0, 128, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_bottom_right",            sf::IntRect(128, 128, 64, 64));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_middle",                  sf::IntRect(64, 64, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_top",                     sf::IntRect(64, 0, 64, 64   ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_left",                    sf::IntRect(0, 64, 64, 64   ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_right",                   sf::IntRect(128, 64, 64, 64 ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_bottom",                  sf::IntRect(64, 128, 64, 64 ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_small_horizontal_left",   sf::IntRect(0, 192, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_small_horizontal_right",  sf::IntRect(128, 192, 64, 64));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_small_horizontal_middle", sf::IntRect(64, 192, 64, 64 ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_small_vertical_top",      sf::IntRect(192, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_small_vertical_bottom",   sf::IntRect(192, 128, 64, 64));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_small_vertical_middle",   sf::IntRect(192, 64, 64, 64 ));
    this->manager->resource.loadTexture("./res/ui/template.png", "widget_base_single",                  sf::IntRect(192, 192, 64, 64));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_top_left",                sf::IntRect(0, 0, 8, 8      ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_top_right",               sf::IntRect(16, 0, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_bottom_left",             sf::IntRect(0, 16, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_bottom_right",            sf::IntRect(16, 16, 8, 8    ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_top",                     sf::IntRect(8, 0, 8, 8      ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_left",                    sf::IntRect(0, 8, 8, 8      ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_right",                   sf::IntRect(16, 8, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_bottom",                  sf::IntRect(8, 16, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_middle",                  sf::IntRect(8, 8, 8, 8      ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_small_horizontal_left",   sf::IntRect(0, 24, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_small_horizontal_right",  sf::IntRect(16, 24, 8, 8    ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_small_horizontal_middle", sf::IntRect(8, 24, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_small_vertical_top",      sf::IntRect(24, 0, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_small_vertical_bottom",   sf::IntRect(24, 16, 8, 8    ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_small_vertical_middle",   sf::IntRect(24, 8, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/buttons.png",  "button_base_single",                  sf::IntRect(24, 24, 8, 8    ));

    this->manager->resource.loadFont("./res/font/proggy.ttf",   "proggy");
    this->manager->resource.loadFont("./res/font/garamond.ttf", "garamond");
}

void Worldmap::updateMousePosition() {
    this->mouse_position_desktop = sf::Mouse::getPosition(*this->manager->window.getWindow());
    this->mouse_position_window  = this->manager->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    
    this->manager->window.getWindow()->setView(this->view_interface);
    this->mouse_position_interface = this->manager->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    this->manager->window.getWindow()->setView(this->view_game);
}

void Worldmap::moveCamera() {
    auto distance = sf::Vector2f(
        (this->position_pressed.x - this->position_released.x) / this->manager->settings.world_panel_size.x / 2,       
        (this->position_pressed.y - this->position_released.y) / this->manager->settings.world_panel_size.y / 2
    );

    // Multipliers for faster camera movement. 
    float x_multiplier = 6.0f;
    float y_multiplier = 6.0f;    

    // Check the horizontal and vertical bounds of the screen.
    // This makes sure that you can not move past the world map.
    if(this->view_game.getCenter().x + (distance.x * x_multiplier) < (this->manager->settings.world_size * this->manager->settings.world_panel_size.x) - this->view_game.getSize().x / 2.f && this->view_game.getCenter().x + (distance.x * x_multiplier) > 0.f + this->view_game.getSize().x / 2.f) {
        this->view_game.move(x_multiplier * distance.x, 0.f);
    }

    if(this->view_game.getCenter().y + (distance.y * y_multiplier) < (this->manager->settings.world_size * this->manager->settings.world_panel_size.y) - this->view_game.getSize().y / 2.f && this->view_game.getCenter().y + (distance.y * y_multiplier) > 0.f + this->view_game.getSize().y / 2.f) {
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
    if(this->view_game.getCenter().x + this->view_game.getSize().x / 2 > this->manager->settings.world_panel_size.x * this->manager->settings.world_size) {
        float offset_x = -(this->view_game.getCenter().x + this->view_game.getSize().x / 2);

        this->view_game.move(offset_x, 0.f);
    } 
    
    // Check if the camera is positioned badly on the bottom of the screen.
    if(this->view_game.getCenter().y + this->view_game.getSize().y / 2 > this->manager->settings.world_panel_size.y * this->manager->settings.world_size) {
        float offset_y = -(this->view_game.getCenter().y + this->view_game.getSize().y / 2);

        this->view_game.move(0.f, offset_y);
    }
}

void Worldmap::update(float time_per_frame) {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();
    this->updateSelectedPanel();

    this->updateUI();
}

void Worldmap::render(float time_per_frame) {
    this->manager->window.clear(sf::Color(50, 50, 50));
    this->manager->window.getWindow()->setView(this->view_game);

    this->renderWorld();
    this->highlightPanel();
    this->drawSelectedPanel();

    this->manager->window.getWindow()->setView(this->view_interface);

    this->renderUI();

    this->manager->window.getWindow()->setView(this->view_game);
    this->manager->window.display();
}

void Worldmap::handleInput() {
    while(this->manager->window.getWindow()->pollEvent(this->event)) {    
        switch(this->event.type) {
            case sf::Event::Closed: {
                this->manager->exitApplication(0);
                break;
            }

            case sf::Event::MouseButtonPressed: {
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    this->mouse_pressed    = true;
                    this->position_pressed = this->mouse_position_window;
                }

                break;
            }

            case sf::Event::MouseButtonReleased: {
                this->mouse_pressed = false;
                this->mouse_drag    = false;

                break;
            }

            case sf::Event::KeyPressed: {
                const float modifier = 2.0f;

                if(this->controls.isKeyPressed("key_open_control_panel"))
                    this->draw_control_panel = !this->draw_control_panel;

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tilde)) {
                    gui::DebugPerformance* widget_performance = static_cast<gui::DebugPerformance*>(this->interface["component_debug_performance"]);
                    widget_performance->show = !widget_performance->show;
                }  
                
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))      this->manager->world.generateWorld();
                
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                    this->view_game.move(0, -this->manager->settings.world_panel_size.y * modifier);

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    this->view_game.move(-this->manager->settings.world_panel_size.x * modifier, 0);

                if(this->view_game.getCenter().y + (this->manager->settings.world_panel_size.y * modifier) <= (this->manager->settings.world_size * this->manager->settings.world_panel_size.y) - this->view_game.getSize().y / 2)
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                        this->view_game.move(0, this->manager->settings.world_panel_size.y * modifier);

                if(this->view_game.getCenter().x + (this->manager->settings.world_panel_size.x * modifier) <= (this->manager->settings.world_size * this->manager->settings.world_panel_size.x) - this->view_game.getSize().x / 2)
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                        this->view_game.move(this->manager->settings.world_panel_size.x * modifier, 0);

                break;
            }

            case sf::Event::MouseMoved: {
                this->mouse_moved = true;

                if(!this->mouse_drag) {
                    this->mouse_drag = false;
                    break;
                }

                this->position_released = this->mouse_position_window;

                // Avoid accidental drags.
                // This also helps to record button presses (without the intention of dragging the screen).
                if(this->mouse_drag && std::abs(this->position_pressed.x - this->position_released.x) > 10 && std::abs(this->position_pressed.y - this->position_released.y) > 10) 
                    this->move_camera = true;

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
        if(this->mouse_moved && this->mouse_pressed) 
            this->mouse_drag = true;

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

    int draw_calls = 0;

    for(Region& panel : this->manager->world.world_map) {        
        sf::Rect region_screen_area(panel.getPosition(), panel.getSize());

        // Draw only the regions which you can see on the screen.
        if(camera_screen_area.intersects(region_screen_area)) {
            sf::RenderStates region_states;
            region_states.texture = &this->manager->resource.getTexture(panel.getTextureName());
            this->manager->window.draw(panel, region_states);
            draw_calls++;

            if(panel.forest.exists()) {
                sf::RenderStates forest_states;
                forest_states.texture = &this->manager->resource.getTexture(panel.forest.getTextureName());
                this->manager->window.draw(panel.forest, forest_states);
                draw_calls++;
            }

            if(panel.river.exists()) {
                sf::RenderStates river_states;
                river_states.texture = &this->manager->resource.getTexture(panel.river.getTextureName());
                this->manager->window.draw(panel.river, river_states);
                draw_calls++;
            }
        }
    }

    this->draw_calls = draw_calls;
}

void Worldmap::selectPanel() {
    if(this->controls.isLeftMouseButtonPressed() && !this->intersectsUI() && !this->mouse_moved && !this->mouse_drag) {
        sf::Vector2i panel_grid_position = sf::Vector2i(
            this->mouse_position_window.x / this->manager->settings.world_panel_size.x,
            this->mouse_position_window.y / this->manager->settings.world_panel_size.y
        );

        const int index = panel_grid_position.y * this->manager->settings.world_size + panel_grid_position.x;
        this->selected_index = index;

        gui::WidgetRegion* widget_region = static_cast<gui::WidgetRegion*>(this->interface["component_widget_region"]);
            widget_region->show = true;
    }
}

void Worldmap::unselectPanel() {
    if(this->controls.isRightMouseButtonPressed() && !this->mouse_moved && !this->mouse_drag && this->selected_index != -1) {
        this->selected_index = -1;
        
        gui::WidgetRegion* widget_region = static_cast<gui::WidgetRegion*>(this->interface["component_widget_region"]);
            widget_region->show = false;
    }
}

void Worldmap::drawSelectedPanel() {
    if(this->current_index != -1 && this->current_index < 0 && this->current_index > this->manager->settings.world_size * this->manager->settings.world_size) {
        sf::Vector2f panel_position = this->manager->world.world_map[this->current_index].getPosition();

        sf::VertexArray selection_indicator(sf::Quads, 4);

        selection_indicator[0].position = panel_position;
        selection_indicator[1].position = panel_position + sf::Vector2f(this->manager->settings.world_panel_size.x, 0);
        selection_indicator[2].position = panel_position + sf::Vector2f(this->manager->settings.world_panel_size.x, this->manager->settings.world_panel_size.y);
        selection_indicator[3].position = panel_position + sf::Vector2f(0, this->manager->settings.world_panel_size.y);
    
        selection_indicator[0].texCoords = sf::Vector2f(0, 0);
        selection_indicator[1].texCoords = sf::Vector2f(this->manager->settings.world_panel_size.x, 0);
        selection_indicator[2].texCoords = sf::Vector2f(this->manager->settings.world_panel_size.x, this->manager->settings.world_panel_size.y);
        selection_indicator[3].texCoords = sf::Vector2f(0, this->manager->settings.world_panel_size.y);

        sf::RenderStates states;
        states.texture = &this->manager->resource.getTexture("panel_highlight");

        this->manager->window.draw(selection_indicator, states);
    }
}

void Worldmap::updateSelectedPanel() {
    sf::Vector2i tile_grid = sf::Vector2i(
        this->mouse_position_window.x / this->manager->settings.world_panel_size.x,
        this->mouse_position_window.y / this->manager->settings.world_panel_size.y
    );
    
    this->current_index = tile_grid.y * this->manager->settings.world_size + tile_grid.x;

    this->selectPanel();
    this->unselectPanel();
}

void Worldmap::highlightPanel() {
    auto panel_grid_position = sf::Vector2i(
        this->mouse_position_window.x / this->manager->settings.world_panel_size.x,
        this->mouse_position_window.y / this->manager->settings.world_panel_size.y
    );

    auto highlight_position = sf::Vector2f(
        panel_grid_position.x * this->manager->settings.world_panel_size.x,
        panel_grid_position.y * this->manager->settings.world_panel_size.y
    );

    sf::VertexArray highlight(sf::Quads, 4);

    highlight[0].position = highlight_position;
    highlight[1].position = highlight_position + sf::Vector2f(this->manager->settings.world_panel_size.x, 0);
    highlight[2].position = highlight_position + sf::Vector2f(this->manager->settings.world_panel_size.x, this->manager->settings.world_panel_size.y);
    highlight[3].position = highlight_position + sf::Vector2f(0, this->manager->settings.world_panel_size.y);

    highlight[0].texCoords = sf::Vector2f(0, 0);
    highlight[1].texCoords = sf::Vector2f(this->manager->settings.world_panel_size.x, 0);
    highlight[2].texCoords = sf::Vector2f(this->manager->settings.world_panel_size.x, this->manager->settings.world_panel_size.y);
    highlight[3].texCoords = sf::Vector2f(0, this->manager->settings.world_panel_size.y);

    sf::RenderStates states;
    states.texture = &this->manager->resource.getTexture("panel_highlight");

    this->manager->window.draw(highlight, states);
}

void Worldmap::createUI() {
    static gui::WidgetRegion widget_region(this->manager);
    this->interface.insert({ widget_region.getWidgetID(), &widget_region });

    static gui::DebugPerformance widget_performance_worldmap(this->manager);
    this->interface.insert({ widget_performance_worldmap.getWidgetID(), &widget_performance_worldmap });
}

void Worldmap::renderUI() {
    for(const auto& pair : this->interface) {
        auto* page = pair.second;
        if(page) {
            if(page->show) {
                page->updateUI();
                this->manager->window.draw(*page);   
            }
        }
    }
}

void Worldmap::updateUI() {
    gui::WidgetRegion* widget_region = (static_cast<gui::WidgetRegion*>(this->interface.at("component_widget_region")));
    gui::Button*       button_travel = (static_cast<gui::Button*>(widget_region->getComponent("button_travel")));

    if(this->selected_index != -1 && this->controls.isLeftMouseButtonPressed() && button_travel->containsPoint(this->mouse_position_interface)) {
        Region& region = this->manager->world.world_map[this->selected_index]; 

        // You check for these things here to avoid calling functions responsible for world generation.
        // Biomes: arctic, ocean and sea do not have planned content.
        if(region.biome.biome_name == BIOME_ARCTIC.biome_name || region.biome.biome_name == BIOME_OCEAN.biome_name || region.biome.biome_name == BIOME_SEA.biome_name) {
            std::cout << "[Worldmap][Button Visit Region]: Requested to generate a region not meant for visiting.\n";
            this->selected_index = -1;
            return;
        }

        this->move_camera   = false;
        this->zoom_camera   = false;
        this->mouse_pressed = false;
        this->mouse_drag    = false;

        if(!this->manager->gamestate.checkGamestateExists("regionmap")) {
            static Regionmap regionmap_gamestate(this->manager);
            this->manager->gamestate.addGamestate("regionmap", regionmap_gamestate);
        }

        // Get a pointer to the gamestate.
        // You do not need to check if it's a nullptr, because it can not be, but do it just to be sure. 
        // It will make debugging easier in case of a unexpected behaviour (function returns nullptr).
        Regionmap* regionmap = static_cast<Regionmap*>(this->manager->gamestate.getGamestateByName("regionmap"));
        if(!regionmap) {
            std::cout << "[Worldmap][Button Visit Region]: Gamestate regionmap is a nullptr.\n";
            this->manager->exitApplication(1);
        }

        // You have to generate the region first, because setCurrentRegion() depends on it being generated.
        if(!region.visited)
            this->manager->world.generateRegion(this->selected_index, region);

        regionmap->setCurrentRegion(this->selected_index);
        this->manager->gamestate.setGamestate("regionmap");
    }
    
    // Rest of the updates.
}

int Worldmap::getCurrentIndex() {
    return this->current_index;
}

int Worldmap::getSelectedIndex() {
    return this->selected_index;
}

bool Worldmap::intersectsUI() {
    for(const auto& pair : this->interface) {
        const auto* component = pair.second;

        if(component) {
            if(component->intersectsUI(this->mouse_position_interface))
                return true;
        }
    }
    
    return false;
}
