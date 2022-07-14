#include "worldmap.hpp"
#include "generationSettings.hpp"

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

    this->resizeViews();

    this->view_interface.setCenter(this->manager->window.windowWidth() / 2, this->manager->window.windowHeight() / 2);
    this->view_game.setCenter(sf::Vector2f(
        world_settings.panelSize() * world_settings.getWorldWidth(),
        world_settings.panelSize() * world_settings.getWorldWidth()
    ));

    this->controls.addKeyMappingCheck("key_f3",                sf::Keyboard::Key::F3);
    this->controls.addKeyMappingCheck("tilde",                 sf::Keyboard::Key::Tilde);
    this->controls.addKeyMappingCheck("key_escape",            sf::Keyboard::Key::Escape);
    this->controls.addKeyMappingCheck("key_regenerate_world",  sf::Keyboard::Key::R);
    this->controls.addKeyMappingCheck("arrow_left",            sf::Keyboard::Key::Left);
    this->controls.addKeyMappingCheck("arrow_right",           sf::Keyboard::Key::Right);
    this->controls.addKeyMappingCheck("arrow_down",            sf::Keyboard::Key::Down);
    this->controls.addKeyMappingCheck("arrow_up",              sf::Keyboard::Key::Up);
    this->controls.addKeyMappingCheck("backspace",             sf::Keyboard::Key::BackSpace);
    this->controls.addKeyMappingCheck("spacebar",              sf::Keyboard::Key::Space);
    this->controls.addKeyMappingCheck("key_screenshot",        sf::Keyboard::Key::F12);
}

void Worldmap::loadResources() {
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_atlas");
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_full"             , sf::IntRect(0, 1152, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_island_0"         , sf::IntRect(0, 1024, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_island_1"         , sf::IntRect(128, 1024, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_island_2"         , sf::IntRect(256, 1024, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_up_0"       , sf::IntRect(0, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_up_1"       , sf::IntRect(128, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_up_2"       , sf::IntRect(256, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_down_0"     , sf::IntRect(0, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_down_1"     , sf::IntRect(128, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_down_2"     , sf::IntRect(256, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_left_0"     , sf::IntRect(0, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_left_1"     , sf::IntRect(128, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_left_2"     , sf::IntRect(256, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_right_0"    , sf::IntRect(0, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_right_1"    , sf::IntRect(128, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_right_2"    , sf::IntRect(256, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_tl_0", sf::IntRect(0, 512, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_tl_1", sf::IntRect(128, 512, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_tl_2", sf::IntRect(256, 512, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_bl_0", sf::IntRect(0, 640, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_bl_1", sf::IntRect(128, 640, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_bl_2", sf::IntRect(256, 640, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_tr_0", sf::IntRect(0, 768, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_tr_1", sf::IntRect(128, 768, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_tr_2", sf::IntRect(256, 768, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_br_0", sf::IntRect(0, 896, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_br_1", sf::IntRect(128, 896, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_corner_br_2", sf::IntRect(256, 896, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_tl_0"  , sf::IntRect(384, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_tl_1"  , sf::IntRect(512, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_tl_2"  , sf::IntRect(640, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_bl_0"  , sf::IntRect(384, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_bl_1"  , sf::IntRect(512, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_bl_2"  , sf::IntRect(640, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_tr_0"  , sf::IntRect(384, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_tr_1"  , sf::IntRect(512, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_tr_2"  , sf::IntRect(640, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_br_0"  , sf::IntRect(384, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_br_1"  , sf::IntRect(512, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas.png", "panel_coast_turn_br_2"  , sf::IntRect(640, 384, 128, 128));

    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_horizontal_0"        , sf::IntRect(0, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_horizontal_1"        , sf::IntRect(128, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_horizontal_2"        , sf::IntRect(256, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_vertical_0"          , sf::IntRect(0, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_vertical_1"          , sf::IntRect(128, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_vertical_2"          , sf::IntRect(256, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_left_0"      , sf::IntRect(0, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_left_1"      , sf::IntRect(128, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_left_2"      , sf::IntRect(256, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_right_0"     , sf::IntRect(0, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_right_1"     , sf::IntRect(128, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_right_2"     , sf::IntRect(256, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_up_0"        , sf::IntRect(0, 512, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_up_1"        , sf::IntRect(128, 512, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_up_2"        , sf::IntRect(256, 512, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_down_0"      , sf::IntRect(0, 640, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_down_1"      , sf::IntRect(128, 640, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_estuary_down_2"      , sf::IntRect(256, 640, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_br_0"           , sf::IntRect(0, 768, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_br_1"           , sf::IntRect(128, 768, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_br_2"           , sf::IntRect(256, 768, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_bl_0"           , sf::IntRect(0, 896, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_bl_1"           , sf::IntRect(128, 896, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_bl_2"           , sf::IntRect(256, 896, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_tl_0"           , sf::IntRect(0, 1024, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_tl_1"           , sf::IntRect(128, 1024, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_tl_2"           , sf::IntRect(256, 1024, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_tr_0"           , sf::IntRect(0, 1152, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_tr_1"           , sf::IntRect(128, 1152, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_turn_tr_2"           , sf::IntRect(256, 1152, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_left_0" , sf::IntRect(384, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_left_1" , sf::IntRect(512, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_left_2" , sf::IntRect(640, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_up_0"   , sf::IntRect(384, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_up_1"   , sf::IntRect(512, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_up_2"   , sf::IntRect(640, 128, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_right_0", sf::IntRect(384, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_right_1", sf::IntRect(512, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_right_2", sf::IntRect(640, 256, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_down_0" , sf::IntRect(384, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_down_1" , sf::IntRect(512, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_river_origin_down_2" , sf::IntRect(640, 384, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_lake_0"              , sf::IntRect(384, 512, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_lake_1"              , sf::IntRect(512, 512, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_river.png", "panel_lake_2"              , sf::IntRect(640, 512, 128, 128));

    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_foliage.png", "panel_tree_continental_1"  , sf::IntRect(0, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_foliage.png", "panel_tree_temperate_1"    , sf::IntRect(128, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_foliage.png", "panel_tree_mediterranean_1", sf::IntRect(256, 0, 128, 128));
    this->manager->resource.loadTexture("./res/worldmap/panel_atlas_foliage.png", "panel_tree_tropical_1"     , sf::IntRect(384, 0, 128, 128));

    this->manager->resource.loadTexture("./res/worldmap/unit_atlas.png", "unit_worldmap_settler", sf::IntRect(0, 0, 128, 128 ));
    this->manager->resource.loadTexture("./res/worldmap/unit_atlas.png", "unit_worldmap_warrior", sf::IntRect(0, 128, 128, 128));

    this->manager->resource.loadTexture("./res/default.png", "default");
}

void Worldmap::moveCamera() {
    auto distance = sf::Vector2f(
        (this->position_pressed.x - this->position_released.x) / world_settings.panelSize() / 2,       
        (this->position_pressed.y - this->position_released.y) / world_settings.panelSize() / 2
    );

    // Multipliers for faster camera movement. 
    const float x_multiplier = (this->current_zoom + 1) * 5.00f;
    const float y_multiplier = (this->current_zoom + 1) * 5.00f;    

    const auto tile_top_left     = this->manager->world.world_map[0];
    const auto tile_bottom_right = this->manager->world.world_map[world_settings.getWorldSize() - 1];
    
    const auto left_bound   = tile_top_left.getPosition2D().x + -(world_settings.getWorldWidth() * world_settings.panelSize() / 8); 
    const auto right_bound  = tile_bottom_right.getPosition2D().x + world_settings.getWorldWidth() * world_settings.panelSize() / 8;
    const auto top_bound    = tile_top_left.getPosition2D().y + -(world_settings.getWorldWidth() * world_settings.panelSize() / 8);
    const auto bottom_bound = tile_bottom_right.getPosition2D().y + world_settings.getWorldWidth() * world_settings.panelSize() / 8;

    if(this->view_game.getCenter().x + distance.x * x_multiplier > left_bound - this->view_game.getSize().x / 4 && this->view_game.getCenter().x + distance.x * x_multiplier < right_bound + this->view_game.getSize().x / 4)
        this->view_game.move(distance.x * x_multiplier, 0);

    if(this->view_game.getCenter().y + distance.y * y_multiplier > top_bound - this->view_game.getSize().y / 4 && this->view_game.getCenter().y + distance.y * y_multiplier < bottom_bound + this->view_game.getSize().y / 4)
        this->view_game.move(0, distance.y * y_multiplier);

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
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();
    
    this->updateUI();
}

void Worldmap::render(float delta_time) {
    this->manager->window.clear(COLOUR_BLACK);
    this->manager->window.getWindow()->setView(this->view_game);

    this->renderWorld();
    this->highlightPanel();

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

            case sf::Event::Resized: {
                auto new_window_size = sf::Vector2f(
                    this->event.size.width,
                    this->event.size.height
                );

                // To avoid weird camera glitches, set the centre explicitly.

                int human_player_capital = this->manager->getHumanPlayer()->getCapital();
                if(human_player_capital == -1) {
                    int index = world_settings.getWorldSize() / 2 - 1;
                    const auto& region = this->manager->world.world_map[index];
                    auto position      = region.getPosition2D();
                    this->view_game.setCenter(position);
                }

                else {
                    const auto& region = this->manager->world.world_map[human_player_capital];
                    auto position      = region.getPosition2D();
                    this->view_game.setCenter(position);
                }

                this->view_interface.setCenter(new_window_size.x / 2, new_window_size.y / 2);
                this->manager->font_size = (this->manager->window.windowWidth() + this->manager->window.windowHeight()) / 160;

                this->manager->window.getWindow()->setView(this->view_interface);
                this->resizeViews();
                this->resizeUI();
                this->manager->window.getWindow()->setView(this->view_game);

                break; 
            }

            case sf::Event::TextEntered: {
                sf::String input(this->event.text.unicode);
                auto readable_text = input.toAnsiString();

                auto* settle_city = static_cast<gui::WidgetSettleCity*>(this->getInterfaceComponent("component_widget_settle_city"));
                if(settle_city->acceptsInput() && settle_city->isVisible())
                    settle_city->updateInput(readable_text);

                break;
            }

            case sf::Event::KeyPressed: {
                if(this->block_keybinds)
                    break;

                for(const auto& pair : this->controls.key_map) {
                    const auto& name  = pair.first;
                    const int   state = this->controls.isKeyPressed(name);

                    if(this->controls.key_state.count(name)) {
                        this->controls.key_state[name] = state;
                    }
                    else this->controls.key_state.insert({ name, state });
                }

                if(this->controls.keyState("spacebar")) {
                    this->centreOnCapital();
                }

                if(this->controls.keyState("key_f3")) {
                    this->toggleComponentVisibility("component_debug_performance");
                }

                if(this->controls.keyState("key_escape")) {
                    this->manager->exitApplication(0);
                }

                if(this->controls.keyState("key_regenerate_world")) {
                    this->manager->prepare();
                }

                if(this->controls.keyState("key_screenshot")) {
                    this->manager->window.takeScreenshot();
                }

                const auto tile_top_left     = this->manager->world.world_map[0];
                const auto tile_bottom_right = this->manager->world.world_map[world_settings.getWorldSize() - 1];
                
                const auto left_bound   = tile_top_left.getPosition2D().x + -(world_settings.getWorldWidth() * world_settings.panelSize() / 8); 
                const auto right_bound  = tile_bottom_right.getPosition2D().x + world_settings.getWorldWidth() * world_settings.panelSize() / 8;
                const auto top_bound    = tile_top_left.getPosition2D().y + -(world_settings.getWorldWidth() * world_settings.panelSize() / 8);
                const auto bottom_bound = tile_bottom_right.getPosition2D().y + world_settings.getWorldWidth() * world_settings.panelSize() / 8;

                if(this->controls.keyState("arrow_left"))
                    if(this->view_game.getCenter().x + world_settings.panelSize() >= left_bound)
                        this->view_game.move(-world_settings.panelSize(), 0);

                if(this->controls.keyState("arrow_right"))
                    if(this->view_game.getCenter().x + world_settings.panelSize() <= right_bound)
                        this->view_game.move(world_settings.panelSize(), 0);

                if(this->controls.keyState("arrow_down"))
                    if(this->view_game.getCenter().y + world_settings.panelSize() <= bottom_bound)
                        this->view_game.move(0, world_settings.panelSize());

                if(this->controls.keyState("arrow_up"))
                    if(this->view_game.getCenter().x + (world_settings.panelSize()) <= top_bound)
                        this->view_game.move(0, -world_settings.panelSize());

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
                
                if(this->controls.mouseMiddlePressed())
                    this->position_pressed = this->mouse_position_window;
                
                Region& current_region = this->manager->world.world_map[this->current_index];
                const bool unit_exists = current_region.isUnitPresent();
                if(this->controls.mouseLeftPressed()) {
                    if(unit_exists && current_region.unit->contains(this->mouse_position_window)) {
                        this->selectUnit();
                    }
    
                    else {
                        this->selectPanel();
                    }
                }
                
                if(this->controls.mouseRightPressed() && this->selected_unit_id != -1) {
                    this->selectUnitGoal();
                }
                
                this->unselectUnit();
                this->unselectPanel();

                break;
            }

            case sf::Event::MouseButtonReleased: {
                this->controls.mouse_left   = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
                this->controls.mouse_right  = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
                this->controls.mouse_middle = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
                
                this->mouse_drag = false;
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

    if(this->mouse_moved && this->controls.mouseMiddlePressed())
        this->mouse_drag = true;

    else {
        this->mouse_drag  = false;
    }
}

void Worldmap::renderWorld() {    
    int gpu_draw_calls = 0;

    const sf::Vector2f camera_size   = this->view_game.getSize();
    const sf::Vector2f camera_centre = this->view_game.getCenter();
    const sf::Vector2f panel_size    = sf::Vector2f(world_settings.panelSize(), world_settings.panelSize());
    sf::Rect camera_screen_area(camera_centre - 0.5f * camera_size, camera_size);
    auto* human_player = this->manager->getHumanPlayer();

    for(int i = 0; i < this->manager->world.world_map.size(); i++) {
        const auto& region = this->manager->world.world_map[i];
        sf::Rect region_screen_space(region.getPosition2D(), region.getSize());

        if(camera_screen_area.intersects(region_screen_space)) {
            if(human_player->discoveredRegion(i)) {
                // Draw panel.

                sf::RenderStates panel_states;
                panel_states.texture = &this->manager->resource.getTexture(region.getTextureName());
                this->manager->window.draw(region, panel_states);
                gpu_draw_calls++;
            
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

            else {
                sf::VertexArray fow(sf::Quads, 4);
                fow[0].position = region.getPosition2D(); 
                fow[1].position = region.getPosition2D() + sf::Vector2f(panel_size.x, 0);
                fow[2].position = region.getPosition2D() + sf::Vector2f(panel_size.x, panel_size.y);
                fow[3].position = region.getPosition2D() + sf::Vector2f(0, panel_size.y);

                fow[0].color = COLOUR_BLACK;
                fow[1].color = COLOUR_BLACK;
                fow[2].color = COLOUR_BLACK;
                fow[3].color = COLOUR_BLACK;
                
                this->manager->window.draw(fow);
            }

            for(const auto& player : this->manager->players) {
                for(const auto& unit : player.units) {
                    if(human_player->discoveredRegion(unit.get()->current_index)) {
                        sf::RenderStates states;
                        states.texture = &this->manager->resource.getTexture(unit.get()->getTextureName());
                        this->manager->window.draw(*unit.get(), states);
                    }
                }
            }
        }
    }
    // This is inefficient. You should not iterate through the world map two times.
    for(const auto& region : this->manager->world.world_map) {
        if(region.isOwned()) {
            sf::VertexArray highlight(sf::Quads, 4);

            highlight[0].position = region.getPosition2D() + sf::Vector2f(0, 0);
            highlight[1].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, 0);
            highlight[2].position = region.getPosition2D() + sf::Vector2f(region.getSize().x, region.getSize().y);
            highlight[3].position = region.getPosition2D() + sf::Vector2f(0, region.getSize().y);

            highlight[0].color = region.owner->getTeamColourTransparent();
            highlight[1].color = region.owner->getTeamColourTransparent();
            highlight[2].color = region.owner->getTeamColourTransparent();
            highlight[3].color = region.owner->getTeamColourTransparent();

            this->manager->window.draw(highlight);
        }
    }

    this->manager->updateDrawCalls(gpu_draw_calls);
}

void Worldmap::selectPanel() {
    auto* human_player = this->manager->getHumanPlayer();
    if(this->controls.mouseLeftPressed() && !this->mouseIntersectsUI() && human_player->discoveredRegion(this->current_index)) {
        const auto& region = this->manager->world.world_map[this->current_index];
        if(region.regiontype.is_ocean()) {
            this->setVisibilityFalse("component_widget_region");
            return;
        }

        sf::Vector2i panel_grid_position = sf::Vector2i(
            this->mouse_position_window.x / world_settings.panelSize(),
            this->mouse_position_window.y / world_settings.panelSize()
        );

        const int index = panel_grid_position.y * world_settings.getWorldWidth() + panel_grid_position.x;
        this->selected_index = index;

        this->setVisibilityFalse("component_widget_unit");
        this->setVisibilityTrue("component_widget_region");
        this->selected_unit_id = -1;
    }
}

void Worldmap::unselectPanel() {
    if(this->controls.mouseRightPressed() && this->selected_index != -1) {
        this->selected_index = -1;
        this->toggleComponentVisibility("component_widget_region");
    }
}

void Worldmap::highlightPanel() {
    sf::Vector2i tile_grid = sf::Vector2i(
        this->mouse_position_window.x / world_settings.panelSize(),
        this->mouse_position_window.y / world_settings.panelSize()
    );
    
    this->current_index = world_settings.calculateWorldIndex(tile_grid.x, tile_grid.y);
    
    if(this->mouse_drag)
        return;
    
    if(!world_settings.inWorldBounds(tile_grid))
        return;

    auto panel_grid_position = sf::Vector2i(
        this->mouse_position_window.x / world_settings.panelSize(),
        this->mouse_position_window.y / world_settings.panelSize()
    );

    auto highlight_position = sf::Vector2f(
        panel_grid_position.x * world_settings.panelSize(),
        panel_grid_position.y * world_settings.panelSize()
    );

    sf::VertexArray highlight(sf::Quads, 4);

    highlight[0].position = highlight_position;
    highlight[1].position = highlight_position + sf::Vector2f(world_settings.panelSize(), 0);
    highlight[2].position = highlight_position + sf::Vector2f(world_settings.panelSize(), world_settings.panelSize());
    highlight[3].position = highlight_position + sf::Vector2f(0, world_settings.panelSize());

    highlight[0].color = COLOUR_WHITE_TRANSPARENT_QUARTER;
    highlight[1].color = COLOUR_WHITE_TRANSPARENT_QUARTER;
    highlight[2].color = COLOUR_WHITE_TRANSPARENT_QUARTER;
    highlight[3].color = COLOUR_WHITE_TRANSPARENT_QUARTER;

    this->manager->window.draw(highlight);
}

void Worldmap::createUI() {
    static gui::WidgetRegion     widget_region(this->manager);
    static gui::DebugPerformance widget_performance_worldmap(this->manager);
    static gui::WidgetUnit       widget_unit(this->manager);
    static gui::WidgetSettleCity widget_settle_city(this->manager);
    static gui::Tooltip          tooltip(this->manager);

    this->addInterfaceComponent(&widget_region);
    this->addInterfaceComponent(&widget_performance_worldmap);
    this->addInterfaceComponent(&widget_unit);
    this->addInterfaceComponent(&widget_settle_city);
    this->addInterfaceComponent(&tooltip);
}

void Worldmap::gamestateLoad() {
    // Centre the camera on current tile.
    // Current tile is the capital (when first loaded in) or a tile the player was visiting.

    auto* human_player = this->manager->getHumanPlayer();
    auto* regionmap = this->manager->gamestate.getGamestateByName("regionmap")
        ? static_cast<Regionmap*>(this->manager->gamestate.getGamestateByName("regionmap"))
        : nullptr;
    
    if(human_player) {
        auto* first_unit = human_player->getUnit("Settler");

        int index = (!regionmap && first_unit)
            ? first_unit->current_index    // If loaded in for the first time.
            : regionmap->getRegionIndex(); // If exiting a region.
        
        const auto& region  = this->manager->world.world_map[index];
        auto position = region.getPosition2D();
        this->view_game.setCenter(position);
    }

    this->manager->window.getWindow()->setView(this->view_interface);
    this->resizeViews();
    this->resizeUI();
    this->manager->window.getWindow()->setView(this->view_game);

    this->mouse_moved = false;
    this->mouse_drag  = false;
}

void Worldmap::gamestateClose() {
    
}

void Worldmap::updateScheduler() {

}

void Worldmap::selectUnit() {
    const auto& region = this->manager->world.world_map[this->current_index];
    auto* unit = region.unit;
    auto* human_player = this->manager->getHumanPlayer();

    if(unit) {
        if(this->controls.mouseLeftPressed() && unit->contains(this->mouse_position_window) && human_player->hasUnit(unit->getID()) && human_player->discoveredRegion(unit->current_index)) {
            this->selected_unit_id = unit->getID();
            this->setVisibilityTrue("component_widget_unit");
            this->setVisibilityFalse("component_widget_region");
        }
    }
}

void Worldmap::unselectUnit() {
    Unit* pawn = nullptr;
    for(const auto& player : this->manager->players) {
        for(auto& unit : player.units) {
            if(unit.get()->getID() == this->selected_unit_id) {
                pawn = unit.get();
                break;
            }
        }
    }

    if(pawn) {
        if(this->controls.mouseLeftPressed() && !this->mouseIntersectsUI() && !pawn->contains(this->mouse_position_window)) {
            this->selected_unit_id = -1;
        }
    }

    if(this->controls.mouseLeftPressed() && this->selected_unit_id == -1) {
        this->setVisibilityFalse("component_widget_unit");
    }
}

void Worldmap::selectUnitGoal() {
    if(this->controls.mouseRightPressed() && this->selected_unit_id != -1) {
        for(const auto& player : this->manager->players) {
            Unit* pawn = nullptr;
            for(auto& unit : player.units) {
                if(this->selected_unit_id == unit.get()->getID()) {
                    pawn = unit.get();
                    break;
                }
            }
    
            if(pawn) {
                pawn->goal = this->current_index;                
                auto path  = this->manager->astar(pawn->current_index, pawn->goal);
                pawn->setNewPath(path);
            }
        }
    }
}

GameObject* Worldmap::getSelectedObject() {
    Region* region = &this->manager->world.world_map[this->current_index];
    if(region->isUnitPresent())
        if(region->unit->contains(this->mouse_position_window))
            return region->unit;
    return region;
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
    auto position = region.getPosition2D() + sf::Vector2f(world_settings.panelSize() / 2, world_settings.panelSize() / 2);

    this->view_game.setCenter(position);
}

void Worldmap::resizeViews() {
    auto window_size = this->manager->window.windowSize();
    this->view_interface.setSize(window_size);
    this->view_game.setSize(sf::Vector2f(
        window_size.x * (this->current_zoom + 1),
        window_size.y * (this->current_zoom + 1)
    ));
}