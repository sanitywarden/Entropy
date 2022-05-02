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

    this->default_zoom = 3;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 3;

    this->view_game.setCenter(sf::Vector2f(
        world_settings.panelSize() * world_settings.getWorldWidth(),
        world_settings.panelSize() * world_settings.getWorldWidth()
    ));

    this->view_game.setSize(sf::Vector2f(
        this->manager->window.windowWidth()  * 2,
        this->manager->window.windowHeight() * 2
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

    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_top_left",                sf::IntRect(0, 0, 64, 64    ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_top_right",               sf::IntRect(128, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_bottom_left",             sf::IntRect(0, 128, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_bottom_right",            sf::IntRect(128, 128, 64, 64));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_middle",                  sf::IntRect(64, 64, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_top",                     sf::IntRect(64, 0, 64, 64   ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_left",                    sf::IntRect(0, 64, 64, 64   ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_right",                   sf::IntRect(128, 64, 64, 64 ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_bottom",                  sf::IntRect(64, 128, 64, 64 ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_horizontal_left",   sf::IntRect(0, 192, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_horizontal_right",  sf::IntRect(128, 192, 64, 64));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_horizontal_middle", sf::IntRect(64, 192, 64, 64 ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_vertical_top",      sf::IntRect(192, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_vertical_bottom",   sf::IntRect(192, 128, 64, 64));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_vertical_middle",   sf::IntRect(192, 64, 64, 64 ));
    this->manager->resource.loadTexture("./res/ui/template/template.png", "widget_base_single",                  sf::IntRect(192, 192, 64, 64));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_top_left",                sf::IntRect(0, 0, 8, 8      ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_top_right",               sf::IntRect(16, 0, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_bottom_left",             sf::IntRect(0, 16, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_bottom_right",            sf::IntRect(16, 16, 8, 8    ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_top",                     sf::IntRect(8, 0, 8, 8      ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_left",                    sf::IntRect(0, 8, 8, 8      ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_right",                   sf::IntRect(16, 8, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_bottom",                  sf::IntRect(8, 16, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_middle",                  sf::IntRect(8, 8, 8, 8      ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_small_horizontal_left",   sf::IntRect(0, 24, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_small_horizontal_right",  sf::IntRect(16, 24, 8, 8    ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_small_horizontal_middle", sf::IntRect(8, 24, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_small_vertical_top",      sf::IntRect(24, 0, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_small_vertical_bottom",   sf::IntRect(24, 16, 8, 8    ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_small_vertical_middle",   sf::IntRect(24, 8, 8, 8     ));
    this->manager->resource.loadTexture("./res/ui/template/buttons.png",  "button_base_single",                  sf::IntRect(24, 24, 8, 8    ));

    this->manager->resource.loadFont("./res/font/proggy.ttf",   "proggy");
    this->manager->resource.loadFont("./res/font/garamond.ttf", "garamond");
}

void Worldmap::moveCamera() {
    auto distance = sf::Vector2f(
        (this->position_pressed.x - this->position_released.x) / world_settings.panelSize() / 2,       
        (this->position_pressed.y - this->position_released.y) / world_settings.panelSize() / 2
    );

    // Multipliers for faster camera movement. 
    const float x_multiplier = (this->default_zoom + 1) * 5.00f;
    const float y_multiplier = (this->default_zoom + 1) * 5.00f;    

    // Check the horizontal and vertical bounds of the screen.
    // This makes sure that you can not move past the world map.
    if(this->view_game.getCenter().x + (distance.x * x_multiplier) < (world_settings.getWorldWidth() * world_settings.panelSize()) - this->view_game.getSize().x / 2.f && this->view_game.getCenter().x + (distance.x * x_multiplier) > 0.f + this->view_game.getSize().x / 2.f) {
        this->view_game.move(x_multiplier * distance.x, 0.f);
    }

    if(this->view_game.getCenter().y + (distance.y * y_multiplier) < (world_settings.getWorldWidth() * world_settings.panelSize()) - this->view_game.getSize().y / 2.f && this->view_game.getCenter().y + (distance.y * y_multiplier) > 0.f + this->view_game.getSize().y / 2.f) {
        this->view_game.move(0.f, y_multiplier * distance.y);
    }

    this->move_camera = false;
}

void Worldmap::zoomCamera() {
    // If you scroll up   - zoom in.
    // If you scroll down - zoom out.

    if((this->default_zoom > this->max_zoom_in && this->controls.mouseMiddleUp()) || (this->default_zoom < this->max_zoom_out && !this->controls.mouseMiddleUp())) {  
        if(this->controls.mouseMiddleUp()) {
            this->default_zoom = this->default_zoom - 1;
            this->view_game.zoom(0.5f);
        }
        
        if(this->controls.mouseMiddleDown()) {
            this->default_zoom = this->default_zoom + 1;
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
    if(this->view_game.getCenter().x + this->view_game.getSize().x / 2 > world_settings.panelSize() * world_settings.getWorldWidth()) {
        float offset_x = -(this->view_game.getCenter().x + this->view_game.getSize().x / 2);

        this->view_game.move(offset_x, 0.f);
    } 
    
    // Check if the camera is positioned badly on the bottom of the screen.
    if(this->view_game.getCenter().y + this->view_game.getSize().y / 2 > world_settings.panelSize() * world_settings.getWorldWidth()) {
        float offset_y = -(this->view_game.getCenter().y + this->view_game.getSize().y / 2);

        this->view_game.move(0.f, offset_y);
    }
}

void Worldmap::update(float delta_time) {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();
    
    this->updateUI();
}

void Worldmap::render(float delta_time) {
    this->manager->window.clear(sf::Color(50, 50, 50));
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

            case sf::Event::KeyPressed: {
                for(const auto& pair : this->controls.key_map) {
                    const auto& name  = pair.first;
                    const int   state = this->controls.isKeyPressed(name);

                    if(this->controls.key_state.count(name))
                        this->controls.key_state[name] = state;
                    else this->controls.key_state.insert({ name, state });
                }

                if(this->controls.keyState("key_tilde")) {
                    this->toggleComponentVisibility("component_debug_performance");
                }

                if(this->controls.keyState("key_escape")) {
                    this->manager->exitApplication(0);
                }

                if(this->controls.keyState("key_regenerate_world")) {
                    this->manager->prepare();
                }

                if(this->controls.keyState("arrow_left"))
                    if(this->view_game.getCenter().x + world_settings.panelSize() >= this->view_game.getSize().x / 2)
                        this->view_game.move(-world_settings.panelSize(), 0);

                if(this->controls.keyState("arrow_right"))
                    if(this->view_game.getCenter().x + world_settings.panelSize() <= (world_settings.getWorldWidth() * world_settings.panelSize()) - this->view_game.getSize().x / 2)
                        this->view_game.move(world_settings.panelSize(), 0);

                if(this->controls.keyState("arrow_down"))
                    if(this->view_game.getCenter().y + world_settings.panelSize() <= (world_settings.getWorldWidth() * world_settings.panelSize()) - this->view_game.getSize().y / 2)
                        this->view_game.move(0, world_settings.panelSize());

                if(this->controls.keyState("arrow_up"))
                    if(this->view_game.getCenter().x + (world_settings.panelSize()) <= (world_settings.getWorldWidth() * world_settings.panelSize()) - this->view_game.getSize().x / 2)
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

    for(const auto& region : this->manager->world.world_map) {
        sf::Rect region_screen_space(region.getPosition2D(), region.getSize());
        
        if(camera_screen_area.intersects(region_screen_space)) {
            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(region.getTextureName());
            this->manager->window.draw(region, states);
            gpu_draw_calls++;
        }
    }

    for(const auto& pair : this->manager->world.forests) {
        const auto& forest = pair.second;
        
        sf::Rect forest_screen_space(forest.getPosition2D(), forest.getSize());

        if(camera_screen_area.intersects(forest_screen_space)) {
            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(forest.getTextureName());
            this->manager->window.draw(forest, states);
            gpu_draw_calls++;
        }
    }

    for(const auto& pair : this->manager->world.rivers) {
        const auto& river = pair.second;

        sf::Rect river_screen_space(river.getPosition2D(), river.getSize());

        if(camera_screen_area.intersects(river_screen_space)) {
            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(river.getTextureName());
            this->manager->window.draw(river, states);
            gpu_draw_calls++;
        }
    }

    for(const auto& pair : this->manager->world.lakes) {
        const auto& lake = pair.second;

        sf::Rect lake_screen_space(lake.getPosition2D(), lake.getSize());

        if(camera_screen_area.intersects(lake_screen_space)) {
            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(lake.getTextureName());
            this->manager->window.draw(lake, states);
            gpu_draw_calls++;
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

    for(const auto& player : this->manager->players) {
        for(const auto& unit : player.units) {
            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(unit.get()->getTextureName());
            this->manager->window.draw(*unit.get(), states);
        }
    }

    this->manager->updateDrawCalls(gpu_draw_calls);
}

void Worldmap::selectPanel() {
    if(this->controls.mouseLeftPressed() && !this->mouseIntersectsUI()) {
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
    
    this->current_index = tile_grid.y * world_settings.getWorldWidth() + tile_grid.x;
    
    if(this->mouse_drag)
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
    static gui::WidgetRegion widget_region(this->manager);
    static gui::DebugPerformance widget_performance_worldmap(this->manager);
    static gui::WidgetUnit widget_unit(this->manager);

    this->addInterfaceComponent(&widget_region);
    this->addInterfaceComponent(&widget_performance_worldmap);
    this->addInterfaceComponent(&widget_unit);
}

void Worldmap::gamestateLoad() {
    /* Centre the camera on current tile.
     * Current tile is the capital (when first loaded in) or a tile the player was visiting. */
    
    #if 0
    auto* regionmap = this->manager->gamestate.getGamestateByName("regionmap")
        ? static_cast<Regionmap*>(this->manager->gamestate.getGamestateByName("regionmap"))
        : nullptr;
    
    if(this->manager->players.size()) {
        const int index = regionmap == nullptr
            ? this->manager->getHumanPlayer().getCapital()
            : regionmap->getRegionIndex();
        
        const auto& tile    = this->manager->world.world_map[index];
        const auto position = sf::Vector2f(tile.getPosition() + sf::Vector2f(world_settings.tileSize().x / 2, world_settings.tileSize().y / 2));
        this->view_game.setCenter(position);
    }
    #endif

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

    if(unit) {
        if(this->controls.mouseLeftPressed() && unit->contains(this->mouse_position_window) && unit->owner_id == this->manager->getHumanPlayer().player_id) {
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