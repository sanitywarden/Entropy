#include "regionmap.hpp"

using namespace iso;

Regionmap::Regionmap() {
    
}

Regionmap::Regionmap(SimulationManager* manager) {
    this->manager  = manager;
    this->state_id = "Regionmap";

    this->region = nullptr;

    this->initialise();
    this->loadResources();
}

Regionmap::~Regionmap() {

}

void Regionmap::initialise() {
    this->mouse_pressed  = false;
    this->mouse_moved    = false;
    this->mouse_drag     = false;
    this->mouse_wheel_up = false;
    this->move_camera    = false;
    this->zoom_camera    = false;

    this->current_index = -1;

    this->zoom = 0;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 1;
    
    this->view_game.setCenter(this->manager->window.windowWidth() / 2, this->manager->window.windowHeight() / 2);
    this->view_game.setSize(this->manager->window.windowSize());

    this->view_interface.setCenter(this->manager->window.windowWidth() / 2, this->manager->window.windowHeight() / 2);
    this->view_interface.setSize(this->manager->window.windowSize());

    this->building_manager = BuildingManager(this->manager);
}

void Regionmap::loadResources() {
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_warm",         sf::IntRect(0, 0, 64, 32   ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_cold",         sf::IntRect(64, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_subtropical",  sf::IntRect(128, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_tropical",     sf::IntRect(192, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_tundra",             sf::IntRect(256, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_arctic",             sf::IntRect(320, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_desert",             sf::IntRect(384, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_ocean",              sf::IntRect(448, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_sea",                sf::IntRect(512, 0, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_highlight",          sf::IntRect(0, 288, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_template_direction", sf::IntRect(64, 288, 64, 32));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_height_dirt",        sf::IntRect(0, 32, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_height_stone",       sf::IntRect(64, 32, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_foliage_atlas.png", "tile_tree_warm1",     sf::IntRect(0, 0, 64, 64   ));
    this->manager->resource.loadTexture("./res/tiles/tile_foliage_atlas.png", "tile_tree_warm2",     sf::IntRect(64, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/tiles/tile_foliage_atlas.png", "tile_tree_cold1",     sf::IntRect(128, 0, 64, 64 ));
    this->manager->resource.loadTexture("./res/tiles/tile_foliage_atlas.png", "tile_tree_tropical1", sf::IntRect(256, 0, 64, 64 ));

    this->manager->resource.loadTexture("./res/building_templates.png", "building_treecutter",  sf::IntRect(0, 0, 128, 128  ));
    this->manager->resource.loadTexture("./res/building_templates.png", "building_stonecutter", sf::IntRect(128, 0, 128, 128));
    this->manager->resource.loadTexture("./res/building_templates.png", "building_house_small", sf::IntRect(0, 128, 64, 64));
}

void Regionmap::update(float time_per_frame) {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();

    this->updateTile();
}

void Regionmap::render(float time_per_frame) {
    this->manager->window.getWindow()->clear(sf::Color(100, 100, 100));    

    this->manager->window.getWindow()->setView(this->view_game);

    this->renderRegion();
    this->higlightTile();

    this->manager->window.getWindow()->setView(this->view_interface);

    this->drawDebugText();

    this->manager->window.getWindow()->display();
}

void Regionmap::handleInput() {
    while(this->manager->window.getWindow()->pollEvent(this->event)) {    
        switch(this->event.type) {
            case sf::Event::Closed: {
                this->manager->exitApplication(0);
                break;
            }

            case sf::Event::KeyPressed: {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
                    this->manager->gamestate.setGamestate("worldmap");
                }

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

            case sf::Event::MouseMoved: {
                this->mouse_moved = true;

                if(!this->mouse_drag) {
                    this->mouse_drag  = false;
                    break;
                }

                this->position_released = this->mouse_position_window;

                // Avoid accidental drags.
                // This also helps to record button presses (without the intention of dragging the screen).
                if(this->mouse_drag && std::abs(this->position_pressed.x - this->position_released.x) > 5 && std::abs(this->position_pressed.y - this->position_released.y) > 5) 
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
    }

    // Mouse button pressed is used in multiple situations.
    // Distinguish from dragging the screen and selecting a option.
    if(this->mouse_moved && this->mouse_pressed) 
        this->mouse_drag = true;

    else {
        this->mouse_drag  = false;
        this->mouse_moved = false;
    }
}

void Regionmap::updateMousePosition() {
    this->manager->window.getWindow()->setView(this->view_game);
    
    this->mouse_position_desktop = sf::Mouse::getPosition(*this->manager->window.getWindow());
    this->mouse_position_window  = this->manager->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    
    this->manager->window.getWindow()->setView(this->view_interface);

    this->mouse_position_interface = this->manager->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    
    this->manager->window.getWindow()->setView(this->view_game);
}

void Regionmap::moveCamera() {
    auto division = this->manager->world.region_settings.tile_size.x + this->manager->world.region_settings.tile_size.y;
    auto distance = sf::Vector2f(
        (this->position_pressed.x - this->position_released.x) / division,       
        (this->position_pressed.y - this->position_released.y) / division
    );

    // Multipliers for faster camera movement. 
    float x_multiplier = 6.0f;
    float y_multiplier = 6.0f;    

    // Check the horizontal and vertical bounds of the screen.
    // This makes sure that you can not move past the world map.
    // if(this->view_game.getCenter().x + (distance.x * x_multiplier) < (this->manager->world_settings.size.x * this->manager->world_settings.panel_size.x) - this->view_game.getSize().x / 2.f && this->view_game.getCenter().x + (distance.x * x_multiplier) > 0.f + this->view_game.getSize().x / 2.f) {
        this->view_game.move(x_multiplier * distance.x, 0.f);
    // }

    // if(this->view_game.getCenter().y + (distance.y * y_multiplier) < (this->manager->world_settings.size.y * this->manager->world_settings.panel_size.y) - this->view_game.getSize().y / 2.f && this->view_game.getCenter().y + (distance.y * y_multiplier) > 0.f + this->view_game.getSize().y / 2.f) {
        this->view_game.move(0.f, y_multiplier * distance.y);
    // }

    this->move_camera = false;
}

void Regionmap::zoomCamera() {
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

void Regionmap::updateCamera() {
    if(this->move_camera) {
        this->moveCamera();
        this->move_camera = false;
    }

    if(this->zoom_camera) {
        this->zoomCamera();
        this->zoom_camera = false;
    }
}

void Regionmap::renderRegion() {
    sf::Vector2f camera_size   = this->view_game.getSize();
    sf::Vector2f camera_centre = this->view_game.getCenter();
    
    sf::Rect camera_screen_area(camera_centre - 0.5f * camera_size, camera_size);

    if(this->region != nullptr) {
        for(int i = 0; i < this->region->map.size(); i++) {
            const auto& tile = this->region->map[i];
           
            sf::Rect region_screen_area(tile.getPosition(), tile.getSize());
            if(camera_screen_area.intersects(region_screen_area)) {
                sf::RenderStates states;
                states.texture = &this->manager->resource.getTexture(tile.getTextureName());
                this->manager->window.draw(tile, states);
            }

            const bool tree_exists = this->region->trees.count(i) ? true : false;
            if(tree_exists) {
                const auto& tree = this->region->trees[i];
                sf::Rect tree_screen_area(tree.getPosition(), tree.getSize());

                if(camera_screen_area.intersects(tree_screen_area)) {
                    sf::RenderStates states;
                    states.texture = &this->manager->resource.getTexture(tree.getTextureName());
                    this->manager->window.draw(tree, states);            
                }
            }

            const bool sides_exist = this->region->sides[i].size() > 0 ? true : false;
            if(sides_exist) {
                for(const auto& side : this->region->sides[i]) {
                    sf::RenderStates states;
                    states.texture = &this->manager->resource.getTexture(side.getTextureName());
                    this->manager->window.draw(side, states);
                }
            }
        }
        
        for(int i = 0; i < this->region->map.size(); i++) {
            const bool building_exists = this->region->buildings.count(i) ? true : false;
            if(building_exists) {
                auto building = this->region->buildings[i];

                sf::RenderStates states;
                states.texture = &this->manager->resource.getTexture(building.getTextureName());
                this->manager->window.draw(building, states);
            }
        }
    }
}

void Regionmap::setCurrentRegion(int region_index) {
    this->region = &this->manager->world.world_map[region_index];

    // Here you can setup what to do when entering a region.
    // For example, centre the camera.
    sf::Vector2f first_tile_position = sf::Vector2f(
        this->region->map[0].getPosition().x + this->manager->world.region_settings.tile_size.x / 2,
        this->region->map[0].getPosition().y
    );

    this->view_game.setCenter(first_tile_position);
}

void Regionmap::higlightTile() {
    const int index = this->manager->world.getTileIndex(this->mouse_position_window, *this->region);
    if(index == -1)
        return;

    this->current_index = index;

    auto tile_size   = this->manager->world.region_settings.tile_size;
    auto tile_offset = this->manager->world.region_settings.tile_offset;
    auto region_size = this->manager->world.region_settings.size;

    sf::Vector2f position_transformed = this->region->map[index].getPosition();

    sf::VertexArray highlight(sf::Quads, 4);

    highlight[0].position = position_transformed; 
    highlight[1].position = position_transformed + sf::Vector2f(tile_size.x, 0);
    highlight[2].position = position_transformed + sf::Vector2f(tile_size.x, tile_size.y);
    highlight[3].position = position_transformed + sf::Vector2f(0, tile_size.y);

    highlight[0].texCoords = sf::Vector2f(0, 0);
    highlight[1].texCoords = sf::Vector2f(tile_size.x, 0);
    highlight[2].texCoords = sf::Vector2f(tile_size.x, tile_size.y);
    highlight[3].texCoords = sf::Vector2f(0, tile_size.y);

    sf::RenderStates states;
    states.texture = &this->manager->resource.getTexture("tile_highlight");

    this->manager->window.draw(highlight, states);
}

void Regionmap::drawDebugText() {
    sf::Text text;
    std::string str;

    str += "Frames per second: " + std::to_string(this->manager->getFramesPerSecond()) + "\n";
    str += "Time per frame: "    + std::to_string(this->manager->getTimePerFrame()) + "ms\n";

    str += "Index:  " + std::to_string(this->current_index) + "\n";
    str += "Height: " + std::to_string(this->region->map[this->current_index].elevation) + "\n";    

    text.setString(str);
    text.setFont(this->manager->resource.getFont("garamond"));
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Black);
    text.setPosition(0, 0);

    this->manager->window.draw(text);
}

Region* Regionmap::getCurrentRegion() {
    return this->region;
}

int Regionmap::getCurrentIndex() {
    return this->current_index;
}

void Regionmap::updateTile() {
    this->controls.addKeyMappingCheck("key_build_building",  sf::Keyboard::Key::B);
    this->controls.addKeyMappingCheck("key_remove_building", sf::Keyboard::Key::D);

    if(this->controls.isKeyPressed("key_build_building")) {
        Building building_adjusted        = BUILDING_HOUSE_SMALL;
        building_adjusted.object_position = this->region->map[this->current_index].getPosition() + sf::Vector2f(0, -building_adjusted.getSize().y / 4 - this->manager->region_settings.tile_size.y / 2);
        building_adjusted.object_position += sf::Vector2f(-1, 1); // Adjust for strange textures.

        this->building_manager.placeBuildingCheck(this->current_index, building_adjusted);
    }

    if(this->controls.isKeyPressed("key_remove_building")) {
        this->building_manager.removeBuilding(this->current_index);
    }
}