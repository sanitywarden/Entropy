#include "worldmap.hpp"
#include "worldData.hpp"
#include "simulationManager.hpp"
#include "colours.hpp"
#include "globalutilities.hpp"

#include <filesystem>

namespace iso {
Worldmap::Worldmap() : Gamestate("Worldmap") 
    , current_index(-1), selected_index(-1), lens("NORMAL")
{
    std::srand(time(0));
    
    this->initialise();
    this->loadResources();   
}        

Worldmap::~Worldmap() 
{}

void Worldmap::initialise() {
    this->default_zoom = 0;
    this->current_zoom = this->default_zoom;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 2;

    this->view_interface.setCenter(game_manager.window.getWindowWidth() / 2, game_manager.window.getWindowHeight() / 2);
    this->view_game.setCenter(sf::Vector2f(
        panelSize() * world_data.w_width,
        panelSize() * world_data.w_width
    ));
}

void Worldmap::loadResources() {
    game_manager.resource.paintTexture("panel_full", "panel_ocean", COLOUR_BLUE_OCEAN, COLOUR_BLUE_OCEAN);
}

void Worldmap::moveCamera() {
    auto distance = sf::Vector2f(
        (this->controls.button_position_pressed.x - this->controls.button_position_released.x) / panelSize() / 2,       
        (this->controls.button_position_pressed.y - this->controls.button_position_released.y) / panelSize() / 2
    );

    // Multipliers for faster camera movement. 
    const float x_multiplier = (this->current_zoom + 1) * 5.00f;
    const float y_multiplier = (this->current_zoom + 1) * 5.00f;    

    const auto tile_top_left     = game_manager.world_map[0];
    const auto tile_bottom_right = game_manager.world_map[getWorldSize() - 1];
    
    auto top_left     = sf::Vector2f(0, 0) - sf::Vector2f(this->view_game.getSize().x / 4, this->view_game.getSize().y / 4);
    auto bottom_right = sf::Vector2f(world_data.w_width * panelSize() + panelSize(), world_data.w_width * panelSize() + panelSize()) + sf::Vector2f(this->view_game.getSize().x / 4, this->view_game.getSize().y / 4);
    
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
    game_manager.window.getWindow()->setView(this->view_game);    
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();   
    this->redrawUI();
}

void Worldmap::render(float delta_time) {
    game_manager.window.clear(COLOUR_BLUE_OCEAN);
    game_manager.window.setView(this->view_game);

    this->renderWorld();
    this->highlightPanel();

    game_manager.window.setView(this->view_interface);

    this->renderUI();

    game_manager.window.setView(this->view_game);
    game_manager.window.display();
}

void Worldmap::handleInput() {
    if(event_queue.size()) {
        const auto& event_name = event_queue.at(0);

        if(event_name == "WINDOW_RESIZE") {
            this->resizeUI();
            this->resizeViews();
        }

        if(event_name == "BUTTON_PRESSED" && !this->controls.isInputBlocked()) {
            if(this->controls.isKeyPressed("ESCAPE")) {
                exitApplication(0);
            }

            if(this->controls.isKeyPressed("R")) {
                game_manager.world_generator.generate();
            }

            if(this->controls.isKeyPressed("F12")) {
                game_manager.window.takeScreenshot();
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

            const auto tile_top_left     = game_manager.world_map[0];
            const auto tile_bottom_right = game_manager.world_map[getWorldSize() - 1];
            
            const auto left_bound   = tile_top_left.getPosition2D().x + -(world_data.w_width * panelSize() / 8); 
            const auto right_bound  = tile_bottom_right.getPosition2D().x + world_data.w_width * panelSize() / 8;
            const auto top_bound    = tile_top_left.getPosition2D().y + -(world_data.w_width * panelSize() / 8);
            const auto bottom_bound = tile_bottom_right.getPosition2D().y + world_data.w_width * panelSize() / 8;

            if(this->controls.isKeyPressed("ARROW_LEFT"))
                if(this->view_game.getCenter().x + panelSize() >= left_bound)
                    this->view_game.move(-panelSize(), 0);

            if(this->controls.isKeyPressed("ARROW_RIGHT"))
                if(this->view_game.getCenter().x + panelSize() <= right_bound)
                    this->view_game.move(panelSize(), 0);

            if(this->controls.isKeyPressed("ARROW_DOWN"))
                if(this->view_game.getCenter().y + panelSize() <= bottom_bound)
                    this->view_game.move(0, panelSize());

            if(this->controls.isKeyPressed("ARROW_UP"))
                if(this->view_game.getCenter().x + (panelSize()) <= top_bound)
                    this->view_game.move(0, -panelSize());
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
    const sf::Vector2f camera_size   = this->view_game.getSize();
    const sf::Vector2f camera_centre = this->view_game.getCenter();
    auto panel_size = core::Vector2f(panelSize(), panelSize());
    sf::Rect camera_screen_area(camera_centre - 0.5f * camera_size, camera_size);
    auto* human_player = game_manager.getHumanPlayer();

    for(int i = 0; i < game_manager.world_map.size(); i++) {
        const auto& region = game_manager.world_map[i];
        sf::Rect region_screen_space(region.getPosition2D().asSFMLVector2f(), region.getSize().asSFMLVector2f());

        if(camera_screen_area.intersects(region_screen_space)) {
            if(human_player->discoveredRegion(i) || !world_data.fog_of_war_enabled && !region.regiontype.is_ocean()) {
                // Draw panel.

                if(this->lens == "NORMAL") {
                    sf::RenderStates panel_states;
                    panel_states.texture = &game_manager.resource.getTexture(region.getTextureName());
                    game_manager.window.draw(region, panel_states);
                }

                else if(this->lens == "LATITUDE") {
                    auto colour = sf::Color(0, 0, region.latitude * 255);
                    sf::VertexArray r(sf::Quads, 4);
    
                    r[0].position = region.getPosition2D().asSFMLVector2f();
                    r[1].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(region.getSize().x, 0); 
                    r[2].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(region.getSize().x, region.getSize().y); 
                    r[3].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(0, region.getSize().y);
    
                    r[0].color = colour;
                    r[1].color = colour;
                    r[2].color = colour;
                    r[3].color = colour;
    
                    game_manager.window.draw(r);
                }

                else if(this->lens == "TEMPERATURE") {
                    auto colour = sf::Color(region.temperature * 255, 0, 0);
                    sf::VertexArray r(sf::Quads, 4);
    
                    r[0].position = region.getPosition2D().asSFMLVector2f();
                    r[1].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(region.getSize().x, 0); 
                    r[2].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(region.getSize().x, region.getSize().y); 
                    r[3].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(0, region.getSize().y);
    
                    r[0].color = colour;
                    r[1].color = colour;
                    r[2].color = colour;
                    r[3].color = colour;
    
                    game_manager.window.draw(r);
                }

                else if(this->lens == "MOISTURE") {
                    auto colour = sf::Color(0, 0, region.moisture * 255);
                    sf::VertexArray r(sf::Quads, 4);
    
                    r[0].position = region.getPosition2D().asSFMLVector2f();
                    r[1].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(region.getSize().x, 0); 
                    r[2].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(region.getSize().x, region.getSize().y); 
                    r[3].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(0, region.getSize().y);
    
                    r[0].color = colour;
                    r[1].color = colour;
                    r[2].color = colour;
                    r[3].color = colour;
    
                    game_manager.window.draw(r);
                }

                else if(this->lens == "HEIGHT") {
                    auto colour = sf::Color(region.height * 255, region.height * 255, region.height * 255);
                    sf::VertexArray r(sf::Quads, 4);
    
                    r[0].position = region.getPosition2D().asSFMLVector2f();
                    r[1].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(region.getSize().x, 0); 
                    r[2].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(region.getSize().x, region.getSize().y); 
                    r[3].position = region.getPosition2D().asSFMLVector2f() + sf::Vector2f(0, region.getSize().y);
    
                    r[0].color = colour;
                    r[1].color = colour;
                    r[2].color = colour;
                    r[3].color = colour;
    
                    game_manager.window.draw(r);
                }

                // Draw panel details.
                // Draw forest.

                if(game_manager.forests.count(i)) {
                    const auto& forest = game_manager.forests[i];
                    sf::RenderStates forest_states;
                    forest_states.texture = &game_manager.resource.getTexture(forest.getTextureName());
                    game_manager.window.draw(forest, forest_states);
                }

                // Draw rivers.

                if(game_manager.rivers.count(i)) {
                    const auto& river = game_manager.rivers[i];

                    sf::RenderStates river_states;
                    river_states.texture = &game_manager.resource.getTexture(river.getTextureName());
                    game_manager.window.draw(river, river_states);
                }

                if(game_manager.lakes.count(i)) {
                    const auto& lake = game_manager.lakes[i];

                    sf::RenderStates lake_states;
                    lake_states.texture = &game_manager.resource.getTexture(lake.getTextureName());
                    game_manager.window.draw(lake, lake_states);
                }
            }
        }
    }
}

void Worldmap::highlightPanel() {
    auto tile_grid = core::Vector2i(
        this->mouse_position_window.x / panelSize(),
        this->mouse_position_window.y / panelSize()
    );
    
    if(!inWorldBounds(tile_grid)) {
        this->current_index = -1;
        return;
    }

    this->current_index = calculateWorldIndex(tile_grid);
    if(this->controls.mouse_dragged)
        return;

    auto panel_grid_position = sf::Vector2i(
        this->mouse_position_window.x / panelSize(),
        this->mouse_position_window.y / panelSize()
    );

    auto highlight_position = sf::Vector2f(
        panel_grid_position.x * panelSize(),
        panel_grid_position.y * panelSize()
    );

    sf::VertexArray highlight(sf::Quads, 4);

    highlight[0].position = highlight_position;
    highlight[1].position = highlight_position + sf::Vector2f(panelSize(), 0);
    highlight[2].position = highlight_position + sf::Vector2f(panelSize(), panelSize());
    highlight[3].position = highlight_position + sf::Vector2f(0, panelSize());

    highlight[0].color = COLOUR_WHITE_TRANSPARENT_QUARTER.asSFMLColour();
    highlight[1].color = COLOUR_WHITE_TRANSPARENT_QUARTER.asSFMLColour();
    highlight[2].color = COLOUR_WHITE_TRANSPARENT_QUARTER.asSFMLColour();
    highlight[3].color = COLOUR_WHITE_TRANSPARENT_QUARTER.asSFMLColour();

    game_manager.window.draw(highlight);
}

void Worldmap::gamestateLoad() {
    game_manager.window.getWindow()->setView(this->view_interface);
    this->resizeViews();
    this->resizeUI();
    game_manager.window.getWindow()->setView(this->view_game);

    this->selected_index = -1;
    this->current_index  = -1;

    for(auto& pair : this->interface) {
        auto* component = pair.second.get();
        this->setVisibilityFalse(component->getWidgetID());
    }

    if(game_manager.gamestate.checkGamestateExists("Regionmap") && game_manager.getHumanPlayer()->hasCapital())
        this->centreOnPlayerCapital();
    else
        this->centreWorldmap();
}

void Worldmap::centreWorldmap() {
    auto index = getWorldSize() / 2 - world_data.w_width / 2;
    const auto& panel = game_manager.world_map.at(index);
    auto position = panel.getPosition2D();
    this->view_game.setCenter(position.asSFMLVector2f()); 
}

void Worldmap::centreOnPlayerCapital() {
    auto index = game_manager.getHumanPlayer()->getCapital();
    const auto& panel = game_manager.world_map.at(index);
    auto position = panel.getPosition2D();
    this->view_game.setCenter(position.asSFMLVector2f());
}

void Worldmap::gamestateClose() {
    for(auto& pair : this->interface)
        this->setVisibilityFalse(pair.first);
}

int Worldmap::L_getCurrentIndex() const {
    return this->current_index;
}

int Worldmap::L_getSelectedIndex() const { 
    return this->selected_index;
}

void Worldmap::L_setSelectedIndex(int index) {
    this->selected_index = index;
}
}