#include "worldmap.hpp"

using namespace iso;

Worldmap::Worldmap(SimulationManager* manager) : Gamestate(manager, "Worldmap") {
    this->manager = manager;
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

    this->controls.addKeyMappingCheck("key_tilde",             sf::Keyboard::Key::Tilde);
    this->controls.addKeyMappingCheck("key_escape",            sf::Keyboard::Key::Escape);
    this->controls.addKeyMappingCheck("key_regenerate_world",  sf::Keyboard::Key::R);
    this->controls.addKeyMappingCheck("arrow_left",            sf::Keyboard::Key::Left);
    this->controls.addKeyMappingCheck("arrow_right",           sf::Keyboard::Key::Right);
    this->controls.addKeyMappingCheck("arrow_down",            sf::Keyboard::Key::Down);
    this->controls.addKeyMappingCheck("arrow_up",              sf::Keyboard::Key::Up);
}

void Worldmap::loadResources() {
    this->manager->resource.loadTexture("./res/panels/panel_atlas.png", "panel_atlas");
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
    
    this->manager->resource.loadTexture("./res/panels/panel_foliage_atlas.png", "panel_foliage_atlas");
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

void Worldmap::moveCamera() {
    auto distance = sf::Vector2f(
        (this->position_pressed.x - this->position_released.x) / this->manager->settings.world_panel_size.x / 2,       
        (this->position_pressed.y - this->position_released.y) / this->manager->settings.world_panel_size.y / 2
    );

    // Multipliers for faster camera movement. 
    const float x_multiplier = 6.0f;
    const float y_multiplier = 6.0f;    

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

    if((this->zoom > this->max_zoom_in && this->controls.mouseMiddleUp()) || (this->zoom < this->max_zoom_out && !this->controls.mouseMiddleUp())) {  
        if(this->controls.mouseMiddleUp()) {
            this->zoom = this->zoom - 1;
            this->view_game.zoom(0.5f);
        }
        
        if(this->controls.mouseMiddleDown()) {
            this->zoom = this->zoom + 1;
            this->view_game.zoom(2.f);
        }
    } 

    this->zoom_camera = false;
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

void Worldmap::update(float delta_time) {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();
    this->updateSelectedPanel();

    this->updateUI();
}

void Worldmap::render(float delta_time) {
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

            case sf::Event::KeyPressed: {
                for(const auto& pair : this->controls.key_map) {
                    const auto& name  = pair.first;
                    const int   state = this->controls.isKeyPressed(name);

                    if(this->controls.key_state.count(name))
                        this->controls.key_state[name] = state;
                    else this->controls.key_state.insert({ name, state });
                }

                if(this->controls.keyState("key_tilde")) {
                    auto widget_debug = static_cast<gui::DebugPerformance*>(this->interface["component_debug_performance"]);
                    widget_debug->show = !widget_debug->show;
                }

                if(this->controls.keyState("key_escape")) {
                    this->manager->exitApplication(0);
                }

                if(this->controls.keyState("key_regenerate_world")) {
                    this->manager->prepare();
                }

                if(this->controls.keyState("arrow_left"))
                    if(this->view_game.getCenter().x + this->manager->settings.world_panel_size.x >= this->view_game.getSize().x / 2)
                        this->view_game.move(-this->manager->settings.world_panel_size.x, 0);

                if(this->controls.keyState("arrow_right"))
                    if(this->view_game.getCenter().x + this->manager->settings.world_panel_size.x <= (this->manager->settings.world_size * this->manager->settings.world_panel_size.x) - this->view_game.getSize().x / 2)
                        this->view_game.move(this->manager->settings.world_panel_size.x, 0);

                if(this->controls.keyState("arrow_down"))
                    if(this->view_game.getCenter().y + this->manager->settings.world_panel_size.y <= (this->manager->settings.world_size * this->manager->settings.world_panel_size.y) - this->view_game.getSize().y / 2)
                        this->view_game.move(0, this->manager->settings.world_panel_size.y);

                if(this->controls.keyState("arrow_up"))
                    if(this->view_game.getCenter().x + (this->manager->settings.world_panel_size.x) <= (this->manager->settings.world_size * this->manager->settings.world_panel_size.x) - this->view_game.getSize().x / 2)
                        this->view_game.move(0, -this->manager->settings.world_panel_size.y);

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
                
                if(this->controls.mouseLeftPressed()) {
                    this->mouse_pressed    = true;
                    this->position_pressed = this->mouse_position_window;
                }
                
                break;
            }

            case sf::Event::MouseButtonReleased: {
                this->controls.mouse_left   = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
                this->controls.mouse_right  = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
                this->controls.mouse_middle = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
                
                this->mouse_drag    = false;
                this->mouse_pressed = false;
                break;
            }

            case sf::Event::MouseWheelScrolled: {
                this->controls.mouse_middle_up   = this->event.mouseWheelScroll.delta ==  1 ? 1 : 0;
                this->controls.mouse_middle_down = this->event.mouseWheelScroll.delta == -1 ? 1 : 0;
                
                if(this->controls.mouse_middle_up || this->controls.mouse_middle_down)
                    this->zoom_camera = true;

                break;
            }

            case sf::Event::MouseMoved: {
                this->mouse_moved = true;
                
                if(!this->mouse_drag)
                    break;

                this->position_released = this->mouse_position_window;
                if(this->mouse_drag && this->mouse_moved && std::abs(this->position_pressed.x - this->position_released.x) > 5 && std::abs(this->position_pressed.y - this->position_released.y) > 5)
                    this->move_camera = true;

                break;
            }
        }
    }

    if(this->mouse_moved && this->mouse_pressed)
        this->mouse_drag = true;

    else {
        this->mouse_drag  = false;
        this->mouse_moved = false;
    }
}

sf::VertexBuffer worldmap_mesh_tile(sf::Quads, sf::VertexBuffer::Usage::Dynamic);
sf::VertexBuffer worldmap_mesh_tree(sf::Quads, sf::VertexBuffer::Usage::Dynamic);
void Worldmap::renderWorld() {
    const auto findTexture = [this](const GameObject& object) -> sf::Vector2f {
        const auto& tile_texture = object.getTextureName();
        return TEXTURE_LOOKUP_TABLE.at(tile_texture);
    };

    const sf::Vector2f camera_size   = this->view_game.getSize();
    const sf::Vector2f camera_centre = this->view_game.getCenter();
    const sf::Vector2f panel_size    = this->manager->settings.world_panel_size;

    sf::Rect camera_screen_area(camera_centre - 0.5f * camera_size, camera_size);

    int gpu_draw_calls = 0;

    if(this->recalculate_mesh) {
        int verticies_index = 0;
        sf::Vertex verticies_worldmap[worldmap_mesh_tile.getVertexCount()];
        for(int index = 0; index < this->manager->world.world_map.size(); index++) {
            sf::Vertex* quad = &verticies_worldmap[verticies_index * 4];

            quad[0].position = this->manager->world.world_map[index].getPosition() + sf::Vector2f(0, 0);
            quad[1].position = this->manager->world.world_map[index].getPosition() + sf::Vector2f(panel_size.x, 0);
            quad[2].position = this->manager->world.world_map[index].getPosition() + sf::Vector2f(panel_size.x, panel_size.y);
            quad[3].position = this->manager->world.world_map[index].getPosition() + sf::Vector2f(0, panel_size.y);
        
            const auto texture_coords = findTexture(this->manager->world.world_map[index]);

            quad[0].texCoords = texture_coords + sf::Vector2f(0, 0);
            quad[1].texCoords = texture_coords + sf::Vector2f(panel_size.x, 0);
            quad[2].texCoords = texture_coords + sf::Vector2f(panel_size.x, panel_size.y);
            quad[3].texCoords = texture_coords + sf::Vector2f(0, panel_size.y);

            verticies_index++;

            const bool river_exists = this->manager->world.rivers.count(index);
            if(river_exists) {
                sf::Vertex* river_quad = &verticies_worldmap[verticies_index * 4];

                river_quad[0].position = this->manager->world.rivers[index].getPosition() + sf::Vector2f(0, 0);
                river_quad[1].position = this->manager->world.rivers[index].getPosition() + sf::Vector2f(panel_size.x, 0);
                river_quad[2].position = this->manager->world.rivers[index].getPosition() + sf::Vector2f(panel_size.x, panel_size.y);
                river_quad[3].position = this->manager->world.rivers[index].getPosition() + sf::Vector2f(0, panel_size.y);
            
                const auto texture_coords = findTexture(this->manager->world.rivers[index]);

                river_quad[0].texCoords = texture_coords + sf::Vector2f(0, 0);
                river_quad[1].texCoords = texture_coords + sf::Vector2f(panel_size.x, 0);
                river_quad[2].texCoords = texture_coords + sf::Vector2f(panel_size.x, panel_size.y);
                river_quad[3].texCoords = texture_coords + sf::Vector2f(0, panel_size.y);

                verticies_index++;
            }
        }

        worldmap_mesh_tile.update(verticies_worldmap);
        this->recalculate_mesh = false;
    }

    if(this->recalculate_tree_mesh) {
        sf::Vertex verticies_forest[worldmap_mesh_tree.getVertexCount()];        
        int verticies_index = 0;
        for(int index = 0; index < this->manager->world.forests.size(); index++) {
            const auto& forest = this->manager->world.forests[index];

            // This should not be a requirement, because the texture should NOT be an asterisk.
            // But something somewhere does not work right, and does not assign texture, or just triggers the default constructor of GameObject.
            // I do not know where it happens so just leave it like this.
            // You use verticies_index instead of index because the indexes will not be contigous.
            // They will not be: 0, 1, 2, 3 - More like: 0, 2, 16, 42.
            if(forest.getTextureName() != "*") {
                sf::Vertex* tree_quad = &verticies_forest[verticies_index * 4];
                
                tree_quad[0].position = forest.getPosition() + sf::Vector2f(0, 0);
                tree_quad[1].position = forest.getPosition() + sf::Vector2f(panel_size.x, 0);
                tree_quad[2].position = forest.getPosition() + sf::Vector2f(panel_size.x, panel_size.y);
                tree_quad[3].position = forest.getPosition() + sf::Vector2f(0, panel_size.y);
            
                const auto texture_coords = findTexture(forest);

                tree_quad[0].texCoords = texture_coords + sf::Vector2f(0, 0);
                tree_quad[1].texCoords = texture_coords + sf::Vector2f(panel_size.x, 0);
                tree_quad[2].texCoords = texture_coords + sf::Vector2f(panel_size.x, panel_size.y);
                tree_quad[3].texCoords = texture_coords + sf::Vector2f(0, panel_size.y);

                verticies_index++;
            }            
        }

        worldmap_mesh_tree.update(verticies_forest);
        this->recalculate_tree_mesh = false;
    }

    sf::RenderStates states_panelmap;
    states_panelmap.texture = &this->manager->resource.getTexture("panel_atlas");
    this->manager->window.draw(worldmap_mesh_tile, states_panelmap);
    gpu_draw_calls++;

    sf::RenderStates states_forestmap;
    states_forestmap.texture = &this->manager->resource.getTexture("panel_foliage_atlas");
    this->manager->window.draw(worldmap_mesh_tree, states_forestmap);
    gpu_draw_calls++;

    this->draw_calls = gpu_draw_calls;
}

void Worldmap::selectPanel() {
    if(this->controls.mouseLeftPressed() && !this->intersectsUI() && !this->mouse_moved && !this->mouse_drag) {
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
    if(this->controls.mouseRightPressed() && !this->mouse_moved && !this->mouse_drag && this->selected_index != -1) {
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
    if(this->mouse_drag)
        return;
    
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
                this->manager->window.draw(*page);   
            }
        }
    }
}

void Worldmap::updateUI() {
    for(const auto& pair : this->interface) {
        auto* page = pair.second;
        if(page) {
            if(page->show) {
                page->updateUI();
                page->functionality();   
            }
        }
    }
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

void Worldmap::gamestateLoad() {
    /* Centre the camera on current tile.
     * Current tile is the capital (when first loaded in) or a tile the player was visiting. */
    
    auto* regionmap = this->manager->gamestate.getGamestateByName("regionmap")
        ? static_cast<Regionmap*>(this->manager->gamestate.getGamestateByName("regionmap"))
        : nullptr;
        
    const int index = regionmap == nullptr
        ? this->manager->getHumanPlayer().getCapital()
        : regionmap->getRegionIndex();
    
    const auto& tile    = this->manager->world.world_map[index];
    const auto position = sf::Vector2f(tile.getPosition() + sf::Vector2f(this->manager->settings.region_tile_size.x / 2, this->manager->settings.region_tile_size.y / 2));
    this->view_game.setCenter(position);

    this->mouse_moved   = false;
    this->mouse_drag    = false;
    this->mouse_pressed = false;

    if(!regionmap) {
        this->recalculate_mesh      = true;
        this->recalculate_tree_mesh = true;
        
        const size_t verticies_worldmap  = 4 * this->manager->world.world_map.size() + 4 * this->manager->world.rivers.size();
        const size_t verticies_forestmap = 4 * this->manager->world.forests.size();
    
        worldmap_mesh_tile.create(verticies_worldmap);
        worldmap_mesh_tree.create(verticies_forestmap);
    }
}

void Worldmap::gamestateClose() {
    this->recalculate_tree_mesh = true;
}

int Worldmap::getDrawCalls() {
    return this->draw_calls;
}