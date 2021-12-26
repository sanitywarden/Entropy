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
    this->draw_calls    = 0;

    this->zoom = 2;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 3;
    
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

    this->manager->resource.loadTexture("./res/building_templates.png", "building_house_small", sf::IntRect(64, 0, 64, 64 ));
    this->manager->resource.loadTexture("./res/building_templates.png", "building_farmland",    sf::IntRect(128, 0, 64, 64));
    this->manager->resource.loadTexture("./res/building_templates.png", "building_quarry",      sf::IntRect(192, 0, 64, 64));
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
    auto division = this->manager->settings.region_tile_size.x + this->manager->settings.region_tile_size.y;
    auto distance = sf::Vector2f(
        (this->position_pressed.x - this->position_released.x) / division,       
        (this->position_pressed.y - this->position_released.y) / division
    );

    // Multipliers for faster camera movement. 
    float x_multiplier = 6.0f;
    float y_multiplier = 6.0f;    

    auto top_tile    = this->region->map[0];
    auto left_tile   = this->region->map[this->manager->world.getRegionSize() - this->manager->settings.region_size];
    auto right_tile  = this->region->map[this->manager->settings.region_size - 1];
    auto bottom_tile = this->region->map[this->manager->world.getRegionSize() - 1];

    const int left_bound   = left_tile.getPosition().x;
    const int right_bound  = right_tile.getPosition().x + this->manager->settings.region_tile_size.x;
    const int top_bound    = top_tile.getPosition().y;
    const int bottom_bound = bottom_tile.getPosition().y + this->manager->settings.region_tile_size.y;

    if(this->view_game.getCenter().x + distance.x * x_multiplier > left_bound - this->view_game.getSize().x / 4 && this->view_game.getCenter().x + distance.x * x_multiplier < right_bound + this->view_game.getSize().x / 4)
        this->view_game.move(distance.x * x_multiplier, 0);

    if(this->view_game.getCenter().y + distance.y * y_multiplier > top_bound - this->view_game.getSize().y / 4 && this->view_game.getCenter().y + distance.y * y_multiplier < bottom_bound + this->view_game.getSize().y / 4)
        this->view_game.move(0, distance.y * y_multiplier);

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
    
    int draw_calls = 0;
    sf::Rect camera_screen_area(camera_centre - 0.5f * camera_size, camera_size);

    if(this->region != nullptr) {
        
        int i = 0;
        for(auto tile_iterator = this->region->map.begin(); tile_iterator != this->region->map.end(); tile_iterator++, i++) {
            const auto& tile = *tile_iterator;
           
            sf::Rect region_screen_area(tile.getPosition(), tile.getSize());
            if(camera_screen_area.intersects(region_screen_area)) {
                sf::RenderStates states;
                states.texture = &this->manager->resource.getTexture(tile.getTextureName());
                this->manager->window.draw(tile, states);
                draw_calls++;
            }

            const bool tree_exists = this->region->trees.count(i) ? true : false;
            if(tree_exists) {
                const auto& tree = this->region->trees[i];
                sf::Rect tree_screen_area(tree.getPosition(), tree.getSize());

                if(camera_screen_area.intersects(tree_screen_area)) {
                    sf::RenderStates states;
                    states.texture = &this->manager->resource.getTexture(tree.getTextureName());
                    this->manager->window.draw(tree, states);     
                    draw_calls++;
                }
            }

            const bool sides_exist = this->region->sides[i].size() > 0 ? true : false;
            if(sides_exist) {
                for(const auto& side : this->region->sides[i]) {
                    sf::Rect side_screen_area(side.getPosition(), side.getSize());

                    if(camera_screen_area.intersects(side_screen_area)) {
                        sf::RenderStates states;
                        states.texture = &this->manager->resource.getTexture(side.getTextureName());
                        this->manager->window.draw(side, states);
                        draw_calls++;
                    }
                }
            }
        }
        
        for(auto building_iterator = this->region->buildings.begin(); building_iterator != this->region->buildings.end(); building_iterator++) {
            Building building = (*building_iterator).second;
            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(building.getTextureName());
            this->manager->window.draw(building, states);
            draw_calls++;
        }
    }

    this->draw_calls = draw_calls;
}

void Regionmap::setCurrentRegion(int region_index) {
    this->region = &this->manager->world.world_map[region_index];

    // Here you can setup what to do when entering a region.
    // For example, centre the camera.
    sf::Vector2f first_tile_position = sf::Vector2f(
        this->region->map[0].getPosition().x + this->manager->settings.region_tile_size.x / 2,
        this->region->map[0].getPosition().y
    );

    this->view_game.setCenter(first_tile_position);
}

void Regionmap::higlightTile() {
    const int index = this->manager->world.getTileIndex(this->mouse_position_window, *this->region);
    if(index == -1)
        return;

    this->current_index = index;

    auto tile_size   = this->manager->settings.region_tile_size;
    auto tile_offset = this->manager->settings.region_tile_offset;
    auto region_size = sf::Vector2f(this->manager->settings.region_size, this->manager->settings.region_size);

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
    str += "Time per frame: " + std::to_string(this->manager->getTimePerFrame()) + "ms\n";
    str += "Draw calls: " + std::to_string(this->draw_calls) + "\n";

    str += "Index:  " + std::to_string(this->current_index) + "\n";
    str += "Height: " + std::to_string(this->region->map[this->current_index].elevation) + "\n";    
    
    str += "\nWood:   " + std::to_string(this->manager->getHumanPlayer().getResourceQuantity(Resource::RESOURCE_WOOD)) + "\n";

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
    this->controls.addKeyMappingCheck("key_build_building",      sf::Keyboard::Key::B);
    this->controls.addKeyMappingCheck("key_remove_building",     sf::Keyboard::Key::D);
    this->controls.addKeyMappingCheck("key_removeresource_tree", sf::Keyboard::Key::R);

    // Temporary key binds. 
    // Delete after UI for buildings is made.
    this->controls.addKeyMappingCheck("key_build_house"   ,   sf::Keyboard::Key::Num2);
    this->controls.addKeyMappingCheck("key_build_farmland",   sf::Keyboard::Key::Num3);
    this->controls.addKeyMappingCheck("key_build_quarry",     sf::Keyboard::Key::Num4);

    if(this->controls.isKeyPressed("key_build_house")) {
        Building building = BUILDING_HOUSE_SMALL;
        this->building_manager.placeBuildingCheck(this->current_index, building);
    }

    if(this->controls.isKeyPressed("key_build_farmland")) {
        Building building = BUILDING_FARMLAND;
        this->building_manager.placeBuildingCheck(this->current_index, building);
    }

    if(this->controls.isKeyPressed("key_build_quarry")) {
        Building building = BUILDING_QUARRY;
        this->building_manager.placeBuildingCheck(this->current_index, building);
    }

    if(this->controls.isKeyPressed("key_remove_building")) {
        Building building = this->region->buildings[this->current_index]; 
        this->manager->getHumanPlayer().addResources(building.getBuildingRefund());
        this->building_manager.removeBuilding(this->current_index);
    }

    if(this->controls.isKeyPressed("key_removeresource_tree")) {
        if(this->region->trees[this->current_index].exists()) {
            this->region->trees.erase(this->current_index);
            this->manager->getHumanPlayer().addResource(Resource::RESOURCE_WOOD, 10);
        }
    }
}