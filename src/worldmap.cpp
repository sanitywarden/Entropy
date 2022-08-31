#include "worldmap.hpp"
#include "generationSettings.hpp"
#include "luadriver.hpp"

using namespace iso;

Worldmap::Worldmap(SimulationManager* manager) : Gamestate(manager, "Worldmap") {
    this->manager = manager;
    std::srand(time(0));

    this->initialise();
    this->loadResources();   
    this->createUI();
}        

Worldmap::~Worldmap() 
{}

void Worldmap::initialise() {
    this->mouse_drag     = false;
    this->mouse_moved    = false;
    this->move_camera    = false;
    this->zoom_camera    = false;

    this->current_index    = -1;
    this->selected_index   = -1;
    this->selected_unit_id = -1;

    this->default_zoom = 0;
    this->current_zoom = this->default_zoom;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 2;

    this->lens = "NORMAL";

    this->resizeViews();

    this->view_interface.setCenter(this->manager->window.getWindowWidth() / 2, this->manager->window.getWindowHeight() / 2);
    this->view_game.setCenter(sf::Vector2f(
        game_settings.panelSize() * game_settings.getWorldWidth(),
        game_settings.panelSize() * game_settings.getWorldWidth()
    ));
}

void Worldmap::loadResources() {
    // Load resources.
    auto driver = lua::driver::Driver::getInstance();
    driver->loadResources(this->manager, "./src/scripts/resource/load_worldmap.lua");
}

void Worldmap::moveCamera() {
    auto distance = sf::Vector2f(
        (this->controls.button_position_pressed.x - this->controls.button_position_released.x) / game_settings.panelSize() / 2,       
        (this->controls.button_position_pressed.y - this->controls.button_position_released.y) / game_settings.panelSize() / 2
    );

    // Multipliers for faster camera movement. 
    const float x_multiplier = (this->current_zoom + 1) * 5.00f;
    const float y_multiplier = (this->current_zoom + 1) * 5.00f;    

    const auto tile_top_left     = this->manager->world.world_map[0];
    const auto tile_bottom_right = this->manager->world.world_map[game_settings.getWorldSize() - 1];
    
    auto top_left     = sf::Vector2f(0, 0) - sf::Vector2f(this->view_game.getSize().x / 4, this->view_game.getSize().y / 4);
    auto bottom_right = sf::Vector2f(game_settings.getWorldWidth() * game_settings.panelSize() + game_settings.panelSize(), game_settings.getWorldWidth() * game_settings.panelSize() + game_settings.panelSize()) + sf::Vector2f(this->view_game.getSize().x / 4, this->view_game.getSize().y / 4);
    
    auto length = sf::Vector2f(std::abs(top_left.x - bottom_right.x), std::abs(top_left.y - bottom_right.y));
    sf::FloatRect rect(top_left, length);
    auto movement_vector = sf::Vector2f(distance.x * x_multiplier, distance.y * y_multiplier);
    auto new_position = sf::Vector2f(this->view_game.getCenter().x + movement_vector.x, this->view_game.getCenter().y + movement_vector.y);
    
    if(rect.contains(sf::Vector2f(new_position.x, this->view_game.getCenter().y)))
        this->view_game.move(movement_vector.x, 0);
    
    if(rect.contains(sf::Vector2f(this->view_game.getCenter().x, new_position.y)))
        this->view_game.move(0, movement_vector.y);

    this->move_camera = false;
}

void Worldmap::zoomCamera() {
    // If you scroll up   - zoom in.
    // If you scroll down - zoom out.

    if((this->current_zoom > this->max_zoom_in && this->controls.mouseMiddleUp()) || (this->current_zoom < this->max_zoom_out && !this->controls.mouseMiddleUp())) {  
        if(this->controls.mouseMiddleUp()) {
            this->current_zoom = this->current_zoom - 1;
            this->view_game.zoom(0.5f);
        }
        
        if(this->controls.mouseMiddleDown()) {
            this->current_zoom = this->current_zoom + 1;
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
}

void Worldmap::update(float delta_time) {
    this->manager->window.getWindow()->setView(this->view_game);    
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();   

    this->manager->window.getWindow()->setView(this->view_interface);
    this->updateUI();
}

void Worldmap::render(float delta_time) {
    this->manager->window.clear(COLOUR_BLUE_OCEAN);
    this->manager->window.getWindow()->setView(this->view_game);

    this->renderWorld();
    this->highlightPanel();

    this->manager->window.getWindow()->setView(this->view_interface);

    this->renderUI();

    this->manager->window.getWindow()->setView(this->view_game);
    this->manager->window.display();
}

void Worldmap::handleInput() {
    if(event_queue.size()) {
        const auto& event_name = event_queue.at(0);

        if(event_name == "WINDOW_RESIZE") {
            auto cwr = this->getInterfaceComponent("component_widget_region");
            auto wr = cwr->getComponent("widget_region");

            this->resizeUI();
            this->resizeViews();
        }

        if(event_name == "BUTTON_PRESSED") {
            if(this->controls.isKeyPressed("F3")) {
                this->toggleComponentVisibility("component_debug_performance");
            }

            if(this->controls.isKeyPressed("SPACEBAR")) {
                this->centreOnCapital();
            }

            if(this->controls.isKeyPressed("ESCAPE")) {
                exitApplication(0);
            }

            if(this->controls.isKeyPressed("R")) {
                this->manager->prepare();
            }

            if(this->controls.isKeyPressed("F12")) {
                this->manager->window.takeScreenshot();
            }

            if(this->controls.isKeyPressed("H"))
                this->lens = "HEIGHT";

            if(this->controls.isKeyPressed("L"))
                this->lens = "LATITUDE";

            if(this->controls.isKeyPressed("N"))
                this->lens = "NORMAL";

            if(this->controls.isKeyPressed("M"))
                this->lens = "MOISTURE";

            if(this->controls.isKeyPressed("T"))
                this->lens = "TEMPERATURE";

            const auto tile_top_left     = this->manager->world.world_map[0];
            const auto tile_bottom_right = this->manager->world.world_map[game_settings.getWorldSize() - 1];
            
            const auto left_bound   = tile_top_left.getPosition2D().x + -(game_settings.getWorldWidth() * game_settings.panelSize() / 8); 
            const auto right_bound  = tile_bottom_right.getPosition2D().x + game_settings.getWorldWidth() * game_settings.panelSize() / 8;
            const auto top_bound    = tile_top_left.getPosition2D().y + -(game_settings.getWorldWidth() * game_settings.panelSize() / 8);
            const auto bottom_bound = tile_bottom_right.getPosition2D().y + game_settings.getWorldWidth() * game_settings.panelSize() / 8;

            if(this->controls.isKeyPressed("ARROW_LEFT"))
                if(this->view_game.getCenter().x + game_settings.panelSize() >= left_bound)
                    this->view_game.move(-game_settings.panelSize(), 0);

            if(this->controls.isKeyPressed("ARROW_RIGHT"))
                if(this->view_game.getCenter().x + game_settings.panelSize() <= right_bound)
                    this->view_game.move(game_settings.panelSize(), 0);

            if(this->controls.isKeyPressed("ARROW_DOWN"))
                if(this->view_game.getCenter().y + game_settings.panelSize() <= bottom_bound)
                    this->view_game.move(0, game_settings.panelSize());

            if(this->controls.isKeyPressed("ARROW_UP"))
                if(this->view_game.getCenter().x + (game_settings.panelSize()) <= top_bound)
                    this->view_game.move(0, -game_settings.panelSize());
        }

        if(event_name == "LMB_PRESSED") {
            this->selectPanel();
        }

        if(event_name == "RMB_PRESSED") {
            this->unselectPanel();
        }

        if(event_name == "MMB_PRESSED") {
        
        }

        if(event_name == "MOUSE_BUTTON_RELEASED") {
        
        }

        if(event_name == "MOUSE_MOVED") {
            auto position_requirement = std::abs(this->controls.button_position_pressed.x - this->controls.button_position_released.x) > 5 && std::abs(this->controls.button_position_pressed.y - this->controls.button_position_released.y) > 5;
            if(this->controls.mouse_dragged && this->controls.mouse_moved && position_requirement)
                this->move_camera = true;   
        }

        if(event_name == "MOUSE_WHEEL_SCROLLED") {
            if(this->controls.mouse_middle_down || this->controls.mouse_middle_up)
                this->zoom_camera = true;
        }
    }
}

void Worldmap::renderWorld() {    
    int gpu_draw_calls = 0;

    const sf::Vector2f camera_size   = this->view_game.getSize();
    const sf::Vector2f camera_centre = this->view_game.getCenter();
    const sf::Vector2f panel_size    = sf::Vector2f(game_settings.panelSize(), game_settings.panelSize());
    sf::Rect camera_screen_area(camera_centre - 0.5f * camera_size, camera_size);
    auto* human_player = this->manager->getHumanPlayer();

    for(int i = 0; i < this->manager->world.world_map.size(); i++) {
        const auto& region = this->manager->world.world_map[i];
        sf::Rect region_screen_space(region.getPosition2D(), region.getSize());

        if(camera_screen_area.intersects(region_screen_space)) {
            if(human_player->discoveredRegion(i) || !game_settings.fogOfWarEnabled() && !region.regiontype.is_ocean()) {
                // Draw panel.

                if(this->lens == "NORMAL") {
                    sf::RenderStates panel_states;
                    panel_states.texture = &this->manager->resource.getTexture(region.getTextureName());
                
                    this->manager->window.draw(region, panel_states);
                    gpu_draw_calls++;
                }

                else if(this->lens == "LATITUDE") {
                    auto colour = sf::Color(0, 0, region.latitude * 255);
                    sf::VertexArray r(sf::Quads, 4);
    
                    r[0].position = region.getPosition2D();
                    r[1].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, 0); 
                    r[2].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, region.getSize().y); 
                    r[3].position = region.getPosition2D() + sf::Vector2f(0, region.getSize().y);
    
                    r[0].color = colour;
                    r[1].color = colour;
                    r[2].color = colour;
                    r[3].color = colour;
    
                    this->manager->window.draw(r);
                    gpu_draw_calls++;
                }

                else if(this->lens == "TEMPERATURE") {
                    auto colour = sf::Color(region.temperature * 255, 0, 0);
                    sf::VertexArray r(sf::Quads, 4);
    
                    r[0].position = region.getPosition2D();
                    r[1].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, 0); 
                    r[2].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, region.getSize().y); 
                    r[3].position = region.getPosition2D() + sf::Vector2f(0, region.getSize().y);
    
                    r[0].color = colour;
                    r[1].color = colour;
                    r[2].color = colour;
                    r[3].color = colour;
    
                    this->manager->window.draw(r);
                    gpu_draw_calls++;
                }

                else if(this->lens == "MOISTURE") {
                    auto colour = sf::Color(0, 0, region.moisture * 255);
                    sf::VertexArray r(sf::Quads, 4);
    
                    r[0].position = region.getPosition2D();
                    r[1].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, 0); 
                    r[2].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, region.getSize().y); 
                    r[3].position = region.getPosition2D() + sf::Vector2f(0, region.getSize().y);
    
                    r[0].color = colour;
                    r[1].color = colour;
                    r[2].color = colour;
                    r[3].color = colour;
    
                    this->manager->window.draw(r);
                    gpu_draw_calls++;
                }

                else if(this->lens == "HEIGHT") {
                    auto colour = sf::Color(region.height * 255, region.height * 255, region.height * 255);
                    sf::VertexArray r(sf::Quads, 4);
    
                    r[0].position = region.getPosition2D();
                    r[1].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, 0); 
                    r[2].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, region.getSize().y); 
                    r[3].position = region.getPosition2D() + sf::Vector2f(0, region.getSize().y);
    
                    r[0].color = colour;
                    r[1].color = colour;
                    r[2].color = colour;
                    r[3].color = colour;
    
                    this->manager->window.draw(r);
                    gpu_draw_calls++;
                }

                // Draw panel details.
                // Draw forest.

                if(this->manager->world.forests.count(i)) {
                    const auto& forest = this->manager->world.forests[i];
                    sf::RenderStates forest_states;
                    forest_states.texture = &this->manager->resource.getTexture(forest.getTextureName());
                    this->manager->window.draw(forest, forest_states);
                    gpu_draw_calls++;
                }

                // Draw rivers.

                if(this->manager->world.rivers.count(i)) {
                    const auto& river = this->manager->world.rivers[i];

                    sf::RenderStates river_states;
                    river_states.texture = &this->manager->resource.getTexture(river.getTextureName());
                    this->manager->window.draw(river, river_states);
                    gpu_draw_calls++;
                }

                if(this->manager->world.lakes.count(i)) {
                    const auto& lake = this->manager->world.lakes[i];

                    sf::RenderStates lake_states;
                    lake_states.texture = &this->manager->resource.getTexture(lake.getTextureName());
                    this->manager->window.draw(lake, lake_states);
                    gpu_draw_calls++;
                }
            }
        }
    }

    this->manager->updateDrawCalls(gpu_draw_calls);
}

void Worldmap::selectPanel() {
    auto* human_player = this->manager->getHumanPlayer();
    if(!this->mouseIntersectsUI() && (human_player->discoveredRegion(this->current_index) || !game_settings.fogOfWarEnabled())) {
        const auto& region = this->manager->world.world_map[this->current_index];
        if(region.regiontype.is_ocean()) {
            this->setVisibilityFalse("component_widget_region");
            return;
        }

        sf::Vector2i panel_grid_position = sf::Vector2i(
            this->mouse_position_window.x / game_settings.panelSize(),
            this->mouse_position_window.y / game_settings.panelSize()
        );

        const int index = panel_grid_position.y * game_settings.getWorldWidth() + panel_grid_position.x;
        this->selected_index = index;

        this->setVisibilityFalse("component_widget_unit");
        this->setVisibilityTrue("component_widget_region");
        this->selected_unit_id = -1;
    }
}

void Worldmap::unselectPanel() {
    if(game_settings.inWorldBounds(this->selected_index)) {
        const auto& region = this->manager->world.world_map[this->selected_index];
        if(!region.contains(this->mouse_position_window)) {
            this->selected_index = -1;
            this->setVisibilityFalse("component_widget_region");
        }
    }
}

void Worldmap::highlightPanel() {
    sf::Vector2i tile_grid = sf::Vector2i(
        this->mouse_position_window.x / game_settings.panelSize(),
        this->mouse_position_window.y / game_settings.panelSize()
    );
    
    this->current_index = game_settings.calculateWorldIndex(tile_grid.x, tile_grid.y);
    
    if(this->mouse_drag)
        return;
    
    if(!game_settings.inWorldBounds(tile_grid))
        return;

    auto panel_grid_position = sf::Vector2i(
        this->mouse_position_window.x / game_settings.panelSize(),
        this->mouse_position_window.y / game_settings.panelSize()
    );

    auto highlight_position = sf::Vector2f(
        panel_grid_position.x * game_settings.panelSize(),
        panel_grid_position.y * game_settings.panelSize()
    );

    sf::VertexArray highlight(sf::Quads, 4);

    highlight[0].position = highlight_position;
    highlight[1].position = highlight_position + sf::Vector2f(game_settings.panelSize(), 0);
    highlight[2].position = highlight_position + sf::Vector2f(game_settings.panelSize(), game_settings.panelSize());
    highlight[3].position = highlight_position + sf::Vector2f(0, game_settings.panelSize());

    highlight[0].color = COLOUR_WHITE_TRANSPARENT_QUARTER;
    highlight[1].color = COLOUR_WHITE_TRANSPARENT_QUARTER;
    highlight[2].color = COLOUR_WHITE_TRANSPARENT_QUARTER;
    highlight[3].color = COLOUR_WHITE_TRANSPARENT_QUARTER;

    this->manager->window.draw(highlight);
}

void Worldmap::createUI() {
    auto widget_region      = gui::InterfaceComponent(new gui::WidgetRegion(this->manager));
    auto widget_performance = gui::InterfaceComponent(new gui::DebugPerformance(this->manager));
    
    this->addInterfaceComponent(widget_region);
    this->addInterfaceComponent(widget_performance);
}

void Worldmap::gamestateLoad() {
    auto* human_player = this->manager->getHumanPlayer();
    auto* regionmap = this->manager->gamestate.getGamestateByName("regionmap")
        ? static_cast<Regionmap*>(this->manager->gamestate.getGamestateByName("regionmap"))
        : nullptr;
    
    const auto& region = this->manager->world.world_map[human_player->spawn_spot_index];
    auto camera_centre = region.getPosition2D() + sf::Vector2f(game_settings.panelSize() / 2, game_settings.panelSize() / 2);
    this->view_game.setCenter(camera_centre);

    this->manager->window.getWindow()->setView(this->view_interface);
    this->resizeViews();
    this->resizeUI();
    this->manager->window.getWindow()->setView(this->view_game);

    this->mouse_moved = false;
    this->mouse_drag  = false;

    this->selected_index = -1;
    this->current_index  = -1;

    for(auto& pair : this->interface) {
        auto* component = pair.second.get();
        this->setVisibilityFalse(component->getWidgetID());
    }
}

void Worldmap::gamestateClose() {
    
}

void Worldmap::updateScheduler() {

}

// void Worldmap::selectUnit() {
    // const auto& region = this->manager->world.world_map[this->current_index];
    // auto* unit = region.unit;
    // auto* human_player = this->manager->getHumanPlayer();

    // if(unit) {
    //     if(this->controls.mouseLeftPressed() && unit->contains(this->mouse_position_window) && human_player->hasUnit(unit->getID()) && human_player->discoveredRegion(unit->current_index)) {
    //         this->selected_unit_id = unit->getID();
    //         this->setVisibilityTrue("component_widget_unit");
    //         this->setVisibilityFalse("component_widget_region");
    //     }
    // }
// }

// void Worldmap::unselectUnit() {
//     Unit* pawn = nullptr;
//     for(const auto& player : this->manager->players) {
//         for(auto& unit : player.units) {
//             if(unit.get()->getID() == this->selected_unit_id) {
//                 pawn = unit.get();
//                 break;
//             }
//         }
//     }

//     if(pawn) {
//         if(this->controls.mouseLeftPressed() && !this->mouseIntersectsUI() && !pawn->contains(this->mouse_position_window)) {
//             this->selected_unit_id = -1;
//         }
//     }

//     if(this->controls.mouseLeftPressed() && this->selected_unit_id == -1) {
//         this->setVisibilityFalse("component_widget_unit");
//     }
// }

// void Worldmap::selectUnitGoal() {
//     if(this->controls.mouseRightPressed() && this->selected_unit_id != -1) {
//         auto* pawn = this->manager->getUnit(this->selected_unit_id);
//         if(pawn) {
//             auto goal = this->current_index; 
//             auto path = this->manager->astar(pawn->current_index, goal);
//             pawn->goal = goal;
//             pawn->setNewPath(path);
//         }           
//     }
// }

GameObject* Worldmap::getSelectedObject() {
    return nullptr;
    // Region* region = &this->manager->world.world_map[this->current_index];
    // if(region->isUnitPresent())
    //     if(region->unit->contains(this->mouse_position_window))
    //         return region->unit;
    // return region;
}

int Worldmap::getSelectedUnitID() {
    return this->selected_unit_id;
}

int Worldmap::getCurrentIndex() {
    return this->current_index;
}

int Worldmap::getSelectedIndex() { 
    return this->selected_index;
}

void Worldmap::centreOnCapital() {
    auto* player = this->manager->getHumanPlayer();

    if(!player)
        return;

    if(!player->hasCapital())
        return;

    auto capital_index = player->getCapital();
    const auto& region = this->manager->world.world_map[capital_index];
    auto position = region.getPosition2D() + sf::Vector2f(game_settings.panelSize() / 2, game_settings.panelSize() / 2);

    this->view_game.setCenter(position);
}