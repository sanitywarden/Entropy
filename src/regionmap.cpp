#include "regionmap.hpp"

using namespace iso;
sf::VertexBuffer mesh_tile(sf::Quads, sf::VertexBuffer::Usage::Dynamic);
sf::VertexBuffer mesh_tree(sf::Quads, sf::VertexBuffer::Usage::Dynamic);

Regionmap::Regionmap(SimulationManager* manager) : Gamestate(manager, "Regionmap") {
    this->manager = manager;
    this->region  = nullptr;

    this->initialise();
    this->loadResources();
    this->createUI();
}

Regionmap::~Regionmap() {

}

void Regionmap::initialise() {
    this->mouse_pressed    = false;
    this->mouse_moved      = false;
    this->mouse_drag       = false;
    this->recalculate_mesh = true;

    this->current_index = -1;
    this->draw_calls    = 0;

    this->zoom = 2;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 3;
    
    this->view_game.setCenter(this->manager->window.windowWidth() / 2, this->manager->window.windowHeight() / 2);
    this->view_game.setSize(this->manager->window.windowSize());

    this->view_interface.setCenter(this->manager->window.windowWidth() / 2, this->manager->window.windowHeight() / 2);
    this->view_interface.setSize(this->manager->window.windowSize());

    this->controls.addKeyMappingCheck("key_tilde",   sf::Keyboard::Key::Tilde);
    this->controls.addKeyMappingCheck("key_escape",  sf::Keyboard::Key::Escape);
    this->controls.addKeyMappingCheck("arrow_left",  sf::Keyboard::Key::Left);
    this->controls.addKeyMappingCheck("arrow_right", sf::Keyboard::Key::Right);
    this->controls.addKeyMappingCheck("arrow_down",  sf::Keyboard::Key::Down);
    this->controls.addKeyMappingCheck("arrow_up",    sf::Keyboard::Key::Up);

    this->controls.addKeyMappingCheck("key_remove_building",     sf::Keyboard::Key::D);
    this->controls.addKeyMappingCheck("key_removeresource_tree", sf::Keyboard::Key::R);
    this->controls.addKeyMappingCheck("key_toggle_buildmenu",    sf::Keyboard::Key::B);

    UpdateUtility update_pawn_movement("update_pawn_movement", 1);
        this->scheduler.insert({ update_pawn_movement.id, update_pawn_movement });
}

void Regionmap::loadResources() {
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_atlas");
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_warm",                       sf::IntRect(0, 0, 64, 32    ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_cold",                       sf::IntRect(64, 0, 64, 32   ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_subtropical",                sf::IntRect(128, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_tropical",                   sf::IntRect(192, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_tundra",                           sf::IntRect(256, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_arctic",                           sf::IntRect(320, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_desert",                           sf::IntRect(384, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_ocean",                            sf::IntRect(448, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_sea",                              sf::IntRect(512, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_highlight",                        sf::IntRect(0, 288, 64, 32  ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_template_direction",               sf::IntRect(64, 288, 64, 32 ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_height_dirt",                      sf::IntRect(0, 32, 64, 32   ));
    this->manager->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_height_stone",                     sf::IntRect(64, 32, 64, 32  ));
    
    this->manager->resource.loadTexture("./res/tiles/tile_foliage_atlas.png", "tile_foliage_atlas");
    this->manager->resource.loadTexture("./res/tiles/tile_foliage_atlas.png", "tile_tree_warm1",               sf::IntRect(0, 0, 64, 64   ));
    this->manager->resource.loadTexture("./res/tiles/tile_foliage_atlas.png", "tile_tree_warm2",               sf::IntRect(64, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/tiles/tile_foliage_atlas.png", "tile_tree_cold1",               sf::IntRect(128, 0, 64, 64 ));
    this->manager->resource.loadTexture("./res/tiles/tile_foliage_atlas.png", "tile_tree_tropical1",           sf::IntRect(256, 0, 64, 64 ));

    this->manager->resource.loadTexture("./res/buildings/buildings_primitive.png", "building_atlas");
    this->manager->resource.loadTexture("./res/buildings/buildings_primitive.png", "building_small_house",     sf::IntRect(0, 0, 64, 64   ));
    this->manager->resource.loadTexture("./res/buildings/buildings_primitive.png", "building_farmland",        sf::IntRect(64, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/buildings/buildings_primitive.png", "building_quarry",          sf::IntRect(128, 0, 64, 64 ));
    this->manager->resource.loadTexture("./res/buildings/buildings_primitive.png", "building_woodcutter",      sf::IntRect(192, 0, 64, 64 ));
    this->manager->resource.loadTexture("./res/buildings/buildings_primitive.png", "building_primitive_house", sf::IntRect(0, 64, 64, 64  ));
    
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_horizontal",     sf::IntRect(0, 0, 64, 32    ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_vertical",       sf::IntRect(0, 32, 64, 32   ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_cross",          sf::IntRect(64, 0, 64, 32   ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_point",          sf::IntRect(64, 32, 64, 32  ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_turn_up",        sf::IntRect(128, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_turn_down",      sf::IntRect(128, 32, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_turn_right",     sf::IntRect(192, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_turn_left",      sf::IntRect(192, 32, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_without_down",   sf::IntRect(256, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_without_top",    sf::IntRect(256, 32, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_without_left",   sf::IntRect(320, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_dirt_without_right",  sf::IntRect(320, 32, 64, 32 ));
    
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_horizontal",    sf::IntRect(0, 64, 64, 32   ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_vertical",      sf::IntRect(0, 96, 64, 32   ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_cross",         sf::IntRect(64, 64, 64, 32  ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_point",         sf::IntRect(64, 96, 64, 32  ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_turn_up",       sf::IntRect(128, 64, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_turn_down",     sf::IntRect(128, 96, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_turn_right",    sf::IntRect(192, 64, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_turn_left",     sf::IntRect(192, 96, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_without_down",  sf::IntRect(256, 64, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_without_top",   sf::IntRect(256, 96, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_without_left",  sf::IntRect(320, 64, 64, 32 ));
    this->manager->resource.loadTexture("./res/buildings/path.png", "path_stone_without_right", sf::IntRect(320, 96, 64, 32 ));


    this->manager->resource.loadTexture("./res/actors/actor.png", "actor");
}

void Regionmap::update(float delta_time) {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();

    this->updateScheduler();
    this->updateTile();
    this->updateUI();
}

void Regionmap::render(float delta_time) {
    this->manager->window.getWindow()->clear(COLOUR_WHITE);    

    this->manager->window.getWindow()->setView(this->view_game);

    this->renderRegion();
    this->higlightTile();

    this->manager->window.getWindow()->setView(this->view_interface);

    this->renderUI();

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
                    this->manager->gamestate.setGamestate("worldmap");
                }

                if(this->controls.keyState("key_toggle_buildmenu")) {
                    auto widget_buildimenu = static_cast<gui::WidgetMenuBuilding*>(this->interface["component_widget_menu_building"]);
                    widget_buildimenu->show = !widget_buildimenu->show;
                }

                if(this->controls.keyState("arrow_left"))
                    if(this->view_game.getCenter().x + this->manager->settings.region_tile_size.x >= -this->view_game.getSize().x / 2)
                        this->view_game.move(-this->manager->settings.region_tile_size.x, 0);

                if(this->controls.keyState("arrow_right"))
                    if(this->view_game.getCenter().x + this->manager->settings.region_tile_size.x <= (this->manager->settings.region_size * this->manager->settings.region_tile_size.x) - this->view_game.getSize().x / 2)
                        this->view_game.move(this->manager->settings.region_tile_size.x, 0);

                if(this->controls.keyState("arrow_down"))
                    if(this->view_game.getCenter().y + this->manager->settings.region_tile_size.y <= (this->manager->settings.region_size * this->manager->settings.region_tile_size.y) - this->view_game.getSize().y / 2)
                        this->view_game.move(0, this->manager->settings.region_tile_size.y);

                if(this->controls.keyState("arrow_up"))
                    if(this->view_game.getCenter().x + (this->manager->settings.region_tile_size.x) <= (this->manager->settings.region_size * this->manager->settings.region_tile_size.x) - this->view_game.getSize().x / 2)
                        this->view_game.move(0, -this->manager->settings.region_tile_size.y);

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

    if((this->zoom > this->max_zoom_in && this->controls.mouseMiddleUp()) || (this->zoom < this->max_zoom_out && !this->controls.mouseMiddleUp())) {  
        if(this->controls.mouseMiddleUp()) {
            this->zoom = this->zoom - 1;
            this->view_game.zoom(0.5f);
        }
        
        else if(this->controls.mouseMiddleDown()) {
            this->zoom = this->zoom + 1;
            this->view_game.zoom(2.f);
        }
    } 

    this->zoom_camera = false;
}

void Regionmap::updateCamera() {
    if(this->move_camera)
        this->moveCamera();

    if(this->zoom_camera)
        this->zoomCamera();
}

void Regionmap::renderRegion() {    
    const auto findTexture = [this](const GameObject& object) -> sf::Vector2f {
        const auto& tile_texture = object.getTextureName();
        return TEXTURE_LOOKUP_TABLE.at(tile_texture);
    };

    const sf::Vector2f camera_size   = this->view_game.getSize();
    const sf::Vector2f camera_centre = this->view_game.getCenter();
    const sf::Rect     camera_screen_area(camera_centre - 0.5f * camera_size, camera_size);
    const sf::Vector2f tile_size = this->manager->settings.region_tile_size;

    int gpu_draw_calls = 0;

    if(this->recalculate_mesh) {
        int verticies_index  = 0;
        int side_vector_size = 0;
        for(int i = 0; i < this->region->sides.size(); i++)
            side_vector_size += this->region->sides[i].size();

        const size_t verticies_tilemap = 4 * this->region->map.size() + 4 * side_vector_size;
        const size_t verticies_treemap = 4 * this->region->map.size();

        sf::Vertex verticies_tiles[verticies_tilemap];
        sf::Vertex verticies_trees[verticies_treemap];

        if(!mesh_tile.getVertexCount())
            mesh_tile.create(verticies_tilemap);

        if(!mesh_tree.getVertexCount())
            mesh_tree.create(verticies_treemap);

        for(int index = 0; index < this->manager->world.getRegionSize(); index++) {
            /* There is no point in checking if this tile is in screen coordinates since this would only slow down the drawing process (due to a lot of stuff to compute).
            * Same goes for sides of the tile. */

            sf::Vertex* quad = &verticies_tiles[verticies_index * 4];

            quad[0].position = this->region->map[index].getPosition() + sf::Vector2f(0, 0);
            quad[1].position = this->region->map[index].getPosition() + sf::Vector2f(tile_size.x, 0);
            quad[2].position = this->region->map[index].getPosition() + sf::Vector2f(tile_size.x, tile_size.y);
            quad[3].position = this->region->map[index].getPosition() + sf::Vector2f(0, tile_size.y);

            const auto texture_coords = findTexture(this->region->map[index]);

            quad[0].texCoords = texture_coords + sf::Vector2f(0, 0);
            quad[1].texCoords = texture_coords + sf::Vector2f(tile_size.x, 0);
            quad[2].texCoords = texture_coords + sf::Vector2f(tile_size.x, tile_size.y);
            quad[3].texCoords = texture_coords + sf::Vector2f(0, tile_size.y);
        
            // Add one to the index because you add the tile.
            verticies_index++;  

            for(int side_index = 0; side_index < this->region->sides[index].size(); side_index++) {
                sf::Vertex* side_quad = &verticies_tiles[verticies_index * 4];

                side_quad[0].position = this->region->sides[index][side_index].getPosition() + sf::Vector2f(0, 0);
                side_quad[1].position = this->region->sides[index][side_index].getPosition() + sf::Vector2f(tile_size.x, 0);
                side_quad[2].position = this->region->sides[index][side_index].getPosition() + sf::Vector2f(tile_size.x, tile_size.y);
                side_quad[3].position = this->region->sides[index][side_index].getPosition() + sf::Vector2f(0, tile_size.y);

                const auto texture_coords = findTexture(this->region->sides[index][side_index]);

                side_quad[0].texCoords = texture_coords + sf::Vector2f(0, 0);
                side_quad[1].texCoords = texture_coords + sf::Vector2f(tile_size.x, 0);
                side_quad[2].texCoords = texture_coords + sf::Vector2f(tile_size.x, tile_size.y);
                side_quad[3].texCoords = texture_coords + sf::Vector2f(0, tile_size.y);
            
                // Add one to the index because you add the side.
                verticies_index++;
            }
        }
    
        for(int y = 0; y < this->manager->settings.region_size; y++) {
            for(int x = 0; x < this->manager->settings.region_size; x++) {
                const int  index       = this->manager->world.rCalculateIndex(x, y); 
                const bool tree_exists = this->region->trees.count(index);
                if(tree_exists) {
                    const auto& tree = this->region->trees[index];                    
                    sf::Vertex* quad = &verticies_trees[index * 4];

                    quad[0].position = tree.getPosition() + sf::Vector2f(0, 0);
                    quad[1].position = tree.getPosition() + sf::Vector2f(tile_size.x, 0);
                    quad[2].position = tree.getPosition() + sf::Vector2f(tile_size.x, tile_size.y * 2);
                    quad[3].position = tree.getPosition() + sf::Vector2f(0, tile_size.y * 2);

                    const auto texture_coords = findTexture(tree);

                    quad[0].texCoords = texture_coords + sf::Vector2f(0, 0);
                    quad[1].texCoords = texture_coords + sf::Vector2f(tile_size.x, 0);
                    quad[2].texCoords = texture_coords + sf::Vector2f(tile_size.x, tile_size.y * 2);
                    quad[3].texCoords = texture_coords + sf::Vector2f(0, tile_size.y * 2);
                }
            }
        }

        mesh_tile.update(verticies_tiles);
        mesh_tree.update(verticies_trees);
        this->recalculate_mesh = false;
    }

    sf::RenderStates states_tiles;
    states_tiles.texture = &this->manager->resource.getTexture("tile_atlas"); 
    this->manager->window.draw(mesh_tile, states_tiles);
    gpu_draw_calls++;

    sf::RenderStates states_trees;
    states_trees.texture = &this->manager->resource.getTexture("tile_foliage_atlas");
    this->manager->window.draw(mesh_tree, states_trees);
    gpu_draw_calls++;

    for(const auto& pair : this->region->buildings) {
        const int   index    = pair.first;
        const auto& building = pair.second;

        sf::Rect building_screen_area(building.getPosition(), building.getSize());
        if(camera_screen_area.intersects(building_screen_area)) {
            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(building.getTextureName());
            this->manager->window.draw(building, states);
            gpu_draw_calls++;
        }
    }

    for(const auto& pawn : this->region->population) {
        sf::Rect pawn_screen_area(pawn.getPosition(), pawn.getSize());
        if(camera_screen_area.intersects(pawn_screen_area)) {
            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(pawn.getTextureName());
            this->manager->window.draw(pawn, states);

            gpu_draw_calls++;
        }
    }

    this->draw_calls = gpu_draw_calls;
}

void Regionmap::setCurrentRegion(int region_index) {
    this->region       = &this->manager->world.world_map[region_index];
    this->region_index = region_index;

    // Here you can setup what to do when entering a region.
    // For example, centre the camera.
    sf::Vector2f first_tile_position = sf::Vector2f(
        this->region->map[0].getPosition().x + this->manager->settings.region_tile_size.x / 2,
        this->region->map[0].getPosition().y
    );

    this->view_game.setCenter(first_tile_position);

    this->region->population.resize(10);
    for(int i = 0; i < this->region->population.size(); i++) {
        auto& pawn = this->region->population[i];
        pawn.object_position = this->region->map[0].getPosition();
    }
}

void Regionmap::higlightTile() {
    const int index = this->manager->world.getTileIndex(this->mouse_position_window, *this->region);
    if(index == -1)
        return;

    if(this->mouse_drag)
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

Region* Regionmap::getCurrentRegion() {
    return this->region;
}

int Regionmap::getRegionIndex() {
    return this->region_index;
}

int Regionmap::getCurrentIndex() {
    return this->current_index;
}

void Regionmap::updateTile() {
    auto building_menu = static_cast<gui::WidgetMenuBuilding*>(this->interface["component_widget_menu_building"]);
    if(building_menu->getBuilding() != BUILDING_EMPTY && !this->intersectsUI() && this->mouse_pressed && !this->mouse_drag && building_menu->show) {
        Building building = building_menu->getBuilding();
        this->region->placeBuildingCheck(building, this->manager->settings, this->current_index);
        this->updatePaths(this->current_index);
    }

    if(this->controls.keyState("key_remove_building")) {
        if(this->region->buildings.count(this->current_index)) {
            this->region->removeBuilding(this->current_index);
            this->updatePaths(this->current_index);
        }
    }

    if(this->controls.keyState("key_removeresource_tree")) {
        if(this->region->trees.count(this->current_index)) {
            this->region->trees.erase(this->current_index);
            this->recalculate_mesh = true; 
        }
    }
}

void Regionmap::updatePaths(int index) {
    if(!this->region->buildings.count(index))
        return;

    // Assigns appropriate path texture based on the path tile's surroundings.
    // Only update the paths if they are adjacent to other paths.
    const auto directions = [&](bool LEFT, bool RIGHT, bool TOP, bool DOWN, Building& building) -> void {
        const std::string path_type = (building == BUILDING_PATH_DIRT) ? "dirt" : "stone"; 
        
        if(LEFT && RIGHT && !TOP && !DOWN)
            building.object_texture_name = "path_" + path_type + "_horizontal";

        else if(!LEFT && !RIGHT && TOP && DOWN)
            building.object_texture_name = "path_" + path_type + "_vertical";

        else if(LEFT && RIGHT && TOP && DOWN)
            building.object_texture_name = "path_" + path_type + "_cross";

        else if(LEFT && !RIGHT && TOP && !DOWN)
            building.object_texture_name = "path_" + path_type + "_turn_up";
    
        else if(!LEFT && RIGHT && !TOP && DOWN)
            building.object_texture_name = "path_" + path_type + "_turn_down";

        else if(!LEFT && RIGHT && TOP && !DOWN)
            building.object_texture_name = "path_" + path_type + "_turn_right";

        else if(LEFT && !RIGHT && !TOP && DOWN)
            building.object_texture_name = "path_" + path_type + "_turn_left";

        else if(LEFT && RIGHT && TOP && !DOWN)
            building.object_texture_name = "path_" + path_type + "_without_down";

        else if(LEFT && RIGHT && !TOP && DOWN)
            building.object_texture_name = "path_" + path_type + "_without_top";

        else if(LEFT && !RIGHT && TOP && DOWN)
            building.object_texture_name = "path_" + path_type + "_without_right";

        else if(!LEFT && RIGHT && TOP && DOWN)
            building.object_texture_name = "path_" + path_type + "_without_left";
    
        else if((LEFT && !RIGHT && !TOP && !DOWN) || (!LEFT && RIGHT && !TOP && !DOWN))
            building.object_texture_name = "path_" + path_type + "_horizontal";

        else if((!LEFT && !RIGHT && !TOP && DOWN) || (!LEFT && !RIGHT && TOP && !DOWN))
            building.object_texture_name = "path_" + path_type + "_vertical";
        
        else building.object_texture_name = "path_" + path_type + "_point";
    };

    // Update the singular tile.
    if(this->region->buildings[index] == BUILDING_PATH_DIRT || this->region->buildings[index] == BUILDING_PATH_STONE) {
        Building& building = this->region->buildings[index];
        
        bool LEFT  = this->region->buildings.count(index - 1) ? true : false;
        bool RIGHT = this->region->buildings.count(index + 1) ? true : false;
        bool TOP   = this->region->buildings.count(index - this->manager->settings.region_size) ? true : false;
        bool DOWN  = this->region->buildings.count(index + this->manager->settings.region_size) ? true : false;
        
        bool VERIFIED_LEFT  = LEFT  ? startsWith(this->region->buildings[index - 1].getTextureName(), "path") : false;
        bool VERIFIED_RIGHT = RIGHT ? startsWith(this->region->buildings[index + 1].getTextureName(), "path") : false;
        bool VERIFIED_TOP   = TOP   ? startsWith(this->region->buildings[index - this->manager->settings.region_size].getTextureName(), "path") : false;
        bool VERIFIED_DOWN  = DOWN  ? startsWith(this->region->buildings[index + this->manager->settings.region_size].getTextureName(), "path") : false;

        directions(VERIFIED_LEFT, VERIFIED_RIGHT, VERIFIED_TOP, VERIFIED_DOWN, building);
    }

    // Update the tiles that might have been affected by the previous change.
    for(auto& pair : this->region->buildings) {
        Building& building = pair.second;
        const int i        = pair.first;

        if(building == BUILDING_PATH_DIRT || building == BUILDING_PATH_STONE) {
            bool LEFT  = this->region->buildings.count(i - 1) ? true : false;
            bool RIGHT = this->region->buildings.count(i + 1) ? true : false;
            bool TOP   = this->region->buildings.count(i - this->manager->settings.region_size) ? true : false;
            bool DOWN  = this->region->buildings.count(i + this->manager->settings.region_size) ? true : false;
            
            bool VERIFIED_LEFT  = LEFT  ? startsWith(this->region->buildings[i - 1].getTextureName(), "path") : false;
            bool VERIFIED_RIGHT = RIGHT ? startsWith(this->region->buildings[i + 1].getTextureName(), "path") : false;
            bool VERIFIED_TOP   = TOP   ? startsWith(this->region->buildings[i - this->manager->settings.region_size].getTextureName(), "path") : false;
            bool VERIFIED_DOWN  = DOWN  ? startsWith(this->region->buildings[i + this->manager->settings.region_size].getTextureName(), "path") : false;

            directions(VERIFIED_LEFT, VERIFIED_RIGHT, VERIFIED_TOP, VERIFIED_DOWN, building);
        }
    }
}

void Regionmap::createUI() {
    static gui::WidgetMenuBuilding widget_menu_building(this->manager);
    this->interface.insert({ widget_menu_building.getWidgetID(), &widget_menu_building });

    static gui::DebugPerformance widget_performance_regionmap(this->manager);
    this->interface.insert({ widget_performance_regionmap.getWidgetID(), &widget_performance_regionmap });
}

void Regionmap::renderUI() {
    for(const auto& pair : this->interface) {
        auto* page = pair.second;
        if(page) {
            if(page->show) {
                this->manager->window.draw(*page);   
            }
        }
    }
}

void Regionmap::updateUI() {
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

bool Regionmap::intersectsUI() {
    for(const auto& pair : this->interface) {
        const auto* component = pair.second;
        if(component) {
            if(component->intersectsUI(this->mouse_position_interface))
                return true;
        }
    }
    
    return false;
}

int Regionmap::getDrawCalls() {
    return this->draw_calls;
}

void Regionmap::updateScheduler() {
    const auto nextMove = [this](Pawn& pawn) -> void {
        int index = pawn.getNextMove();
        pawn.object_position = this->region->map[index].getPosition();
    };

    // Update pawns movement.
    auto& update_pawn = this->scheduler["update_pawn_movement"];
    update_pawn.counter++;
    const int fps_value = this->manager->getAverageFramesPerSecond() != 0 ? this->manager->getAverageFramesPerSecond() : this->manager->getFramesPerSecond();
    
    if(update_pawn.counter >= update_pawn.interval * fps_value && this->region->population.size()) {
        for(auto& pawn : this->region->population) {
            if(!pawn.path.empty()) {
                nextMove(pawn);
                update_pawn.counter = 0;
            }
        
            else if(pawn.path.empty()) {    
                int goal = std::rand() % this->manager->world.getRegionSize() - 1;                
                auto path = this->manager->astar(pawn.current_index, goal);
                pawn.setNewPath(path);
            }
        }
    }
}
