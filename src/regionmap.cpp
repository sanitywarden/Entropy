#include "regionmap.hpp"
#include "generationSettings.hpp"
#include "globalutilities.hpp"

using namespace iso;

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
    this->mouse_moved = false;
    this->mouse_drag  = false;

    this->current_index = -1;

    this->default_zoom  = 0;
    this->current_index = this->default_zoom;
    this->max_zoom_in   = 0; 
    this->max_zoom_out  = 3;
    
    this->view_interface.setCenter(this->manager->window.windowWidth() / 2, this->manager->window.windowHeight() / 2);
    this->view_game.setCenter(this->manager->window.windowWidth() / 2, this->manager->window.windowHeight() / 2);

    this->controls.addKeyMappingCheck("key_f3",      sf::Keyboard::Key::F3);
    this->controls.addKeyMappingCheck("key_escape",  sf::Keyboard::Key::Escape);
    this->controls.addKeyMappingCheck("arrow_left",  sf::Keyboard::Key::Left);
    this->controls.addKeyMappingCheck("arrow_right", sf::Keyboard::Key::Right);
    this->controls.addKeyMappingCheck("arrow_down",  sf::Keyboard::Key::Down);
    this->controls.addKeyMappingCheck("arrow_up",    sf::Keyboard::Key::Up);

    this->controls.addKeyMappingCheck("key_screenshot",          sf::Keyboard::Key::F12);
    this->controls.addKeyMappingCheck("key_remove_building",     sf::Keyboard::Key::D);
    this->controls.addKeyMappingCheck("key_removeresource_tree", sf::Keyboard::Key::R);
    this->controls.addKeyMappingCheck("key_toggle_buildmenu",    sf::Keyboard::Key::B);
    this->controls.addKeyMappingCheck("key_toggle_storage",      sf::Keyboard::Key::I);
    this->controls.addKeyMappingCheck("key_toggle_minimap",      sf::Keyboard::Key::M);
    this->controls.addKeyMappingCheck("key_centre_view",         sf::Keyboard::Key::Space);

    this->scheduler.insert({ "update_path"    , std::pair(0, 10) });
    this->scheduler.insert({ "update_movement", std::pair(0, 1)  });
}

void Regionmap::loadResources() {
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_atlas");
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_grass_warm",         sf::IntRect(0, 0, 64, 32    ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_grass_cold",         sf::IntRect(64, 0, 64, 32   ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_grass_subtropical",  sf::IntRect(128, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_grass_tropical",     sf::IntRect(192, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_grass_tundra",       sf::IntRect(256, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_ocean",              sf::IntRect(320, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_river",              sf::IntRect(384, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_river_pass",         sf::IntRect(448, 0, 64, 32  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_black",              sf::IntRect(0, 288, 64, 32  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_black_empty",        sf::IntRect(0, 256, 64, 32  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_template_direction", sf::IntRect(64, 288, 64, 32 ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_height_dirt",        sf::IntRect(0, 32, 64, 32   ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_height_stone",       sf::IntRect(64, 32, 64, 32  ));
    
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_resource_stone" ,   sf::IntRect(0, 64, 64, 32   ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_resource_flint" ,   sf::IntRect(64, 64, 64, 32  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_resource_clay"  ,   sf::IntRect(128, 64, 64, 32 ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_resource_copper",   sf::IntRect(192, 64, 64, 32 ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas.png", "tile_resource_tin"   ,   sf::IntRect(256, 64, 64, 32 ));

    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_foliage_atlas");
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_tree_beech"   , sf::IntRect(0, 0, 64, 96    ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_tree_oak"     , sf::IntRect(64, 0, 64, 96   ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_tree_maple"   , sf::IntRect(128, 0, 64, 96  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_tree_spruce_1", sf::IntRect(0, 96, 64, 96   ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_tree_spruce_2", sf::IntRect(64, 96, 64, 96  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_tree_cypress" , sf::IntRect(0, 192, 64, 96  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_tree_acacia"  , sf::IntRect(64, 192, 128, 96));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_tree_palm"    , sf::IntRect(0, 288, 64, 96  ));
    this->manager->resource.loadTexture("./res/regionmap/tile_atlas_foliage.png", "tile_tree_pine"    , sf::IntRect(704, 0, 64, 192 ));

    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_atlas");
    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_house"          , sf::IntRect(0, 0, 128, 128  ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_hunter"         , sf::IntRect(128, 0, 128, 128));
    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_woodcutter"     , sf::IntRect(256, 0, 128, 128));
    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_quarry"         , sf::IntRect(384, 0, 128, 128));
    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_flint_collector", sf::IntRect(512, 0, 128, 128));
    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_gatherer"       , sf::IntRect(640, 0, 128, 128));
    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_water_collector", sf::IntRect(768, 0, 64, 64));
    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_well"           , sf::IntRect(832, 0, 64, 64));
    this->manager->resource.loadTexture("./res/regionmap/buildings/buildings_primitive.png", "building_toolmaker"      , sf::IntRect(896, 0, 128, 128));

    this->manager->resource.loadTexture("./res/regionmap/buildings/animalspot_deer.png", "animalspot_deer", sf::IntRect(0, 0, 128, 128));

    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_horizontal",     sf::IntRect(0, 0, 64, 64    ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_vertical",       sf::IntRect(0, 64, 64, 64   ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_cross",          sf::IntRect(64, 0, 64, 64   ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_point",          sf::IntRect(64, 64, 64, 64  ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_turn_up",        sf::IntRect(128, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_turn_down",      sf::IntRect(128, 64, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_turn_right",     sf::IntRect(192, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_turn_left",      sf::IntRect(192, 64, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_without_down",   sf::IntRect(256, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_without_top",    sf::IntRect(256, 64, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_without_left",   sf::IntRect(320, 0, 64, 64  ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_dirt_without_right",  sf::IntRect(320, 64, 64, 64 ));

    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_horizontal",    sf::IntRect(0, 128, 64, 64   ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_vertical",      sf::IntRect(0, 192, 64, 64   ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_cross",         sf::IntRect(64, 128, 64, 64  ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_point",         sf::IntRect(64, 192, 64, 64  ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_turn_up",       sf::IntRect(128, 128, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_turn_down",     sf::IntRect(128, 192, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_turn_right",    sf::IntRect(192, 128, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_turn_left",     sf::IntRect(192, 192, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_without_down",  sf::IntRect(256, 128, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_without_top",   sf::IntRect(256, 192, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_without_left",  sf::IntRect(320, 128, 64, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/buildings/path.png", "path_stone_without_right", sf::IntRect(320, 192, 64, 64 ));

    this->manager->resource.loadTexture("./res/ui/template/icon_template.png"                 , "icon_default",                           sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_path_dirt.png"               , "icon_template_path_dirt" ,               sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_path_stone.png"              , "icon_template_path_stone",               sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_building_farmhouse.png"      , "icon_template_building_gatherer",        sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_building_woodcutter.png"     , "icon_template_building_woodcutter",      sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_building_quarry.png"         , "icon_template_building_quarry",          sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_building_house.png"          , "icon_template_building_house",           sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_building_flint.png"          , "icon_template_building_flint_collector", sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_building_hunter.png"         , "icon_template_building_hunter",          sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_building_toolmaker.png"      , "icon_template_building_toolmaker",       sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_building_well.png"           , "icon_template_building_well",            sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/buildings/icon_building_water_collector.png", "icon_template_building_water_collector", sf::IntRect(0, 0, 48, 48));

    this->manager->resource.loadTexture("./res/ui/items/icon_item_flint.png"  , "icon_template_item_flint"  , sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/items/icon_item_stone.png"  , "icon_template_item_stone"  , sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/items/icon_item_wood.png"   , "icon_template_item_wood"   , sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/items/icon_item_leather.png", "icon_template_item_leather", sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/items/icon_item_meat.png"   , "icon_template_item_meat"   , sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/items/icon_item_grain.png"  , "icon_template_item_grain"  , sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/items/icon_item_water.png"  , "icon_template_item_water"  , sf::IntRect(0, 0, 48, 48));
    this->manager->resource.loadTexture("./res/ui/items/icon_item_tools.png"  , "icon_template_item_tools"  , sf::IntRect(0, 0, 48, 48));

    this->manager->resource.loadTexture("./res/regionmap/building_size_highlight_template.png", "tile_black_1x1", sf::IntRect(0,   0, 64,  32 ));
    this->manager->resource.loadTexture("./res/regionmap/building_size_highlight_template.png", "tile_black_2x2", sf::IntRect(64,  0, 128, 64 ));
    this->manager->resource.loadTexture("./res/regionmap/building_size_highlight_template.png", "tile_black_3x3", sf::IntRect(192, 0, 192, 96));
    this->manager->resource.loadTexture("./res/regionmap/building_size_highlight_template.png", "tile_black_4x4", sf::IntRect(384, 0, 256, 128));

    this->manager->resource.loadTexture("./res/regionmap/units/unit_classic.png", "unit_classic_arrow_tl", sf::IntRect(0  , 0, 64, 96));
    this->manager->resource.loadTexture("./res/regionmap/units/unit_classic.png", "unit_classic_arrow_tr", sf::IntRect(64 , 0, 64, 96));
    this->manager->resource.loadTexture("./res/regionmap/units/unit_classic.png", "unit_classic_arrow_br", sf::IntRect(128, 0, 64, 96));
    this->manager->resource.loadTexture("./res/regionmap/units/unit_classic.png", "unit_classic_arrow_bl", sf::IntRect(192, 0, 64, 96));

    this->manager->resource.loadTexture("./res/regionmap/units/unit_classic.png", "unit_transport_arrow_tl", sf::IntRect(0  , 96, 64, 96));
    this->manager->resource.loadTexture("./res/regionmap/units/unit_classic.png", "unit_transport_arrow_tr", sf::IntRect(64 , 96, 64, 96));
    this->manager->resource.loadTexture("./res/regionmap/units/unit_classic.png", "unit_transport_arrow_br", sf::IntRect(128, 96, 64, 96));
    this->manager->resource.loadTexture("./res/regionmap/units/unit_classic.png", "unit_transport_arrow_bl", sf::IntRect(192, 96, 64, 96));

    this->manager->texturizer.createColouredWorldmapTexture("tile_black_1x1", "tile_highlight_1x1"    , COLOUR_WHITE_TRANSPARENT_HALF, COLOUR_TRANSPARENT);
    this->manager->texturizer.createColouredWorldmapTexture("tile_black_1x1", "tile_transparent_white", COLOUR_WHITE_TRANSPARENT_HALF, COLOUR_TRANSPARENT);
    this->manager->texturizer.createColouredWorldmapTexture("tile_black_1x1", "tile_transparent_green", COLOUR_GREEN_TRANSPARENT_HALF, COLOUR_TRANSPARENT);
    this->manager->texturizer.createColouredWorldmapTexture("tile_black_1x1", "tile_transparent_red"  , COLOUR_RED_TRANSPARENT_HALF  , COLOUR_TRANSPARENT);

    // Automatically generate icon names.
    for(const auto& texture_pair : this->manager->resource.getTextureCollection()) {
        const auto& texture_name = texture_pair.first;
        const auto& texture      = texture_pair.second;

        // Generate icons.
        if(startsWith(texture_name, "icon_template_")) {
            auto save_as = "icon_" + readAfter(texture_name, "icon_template_");
            auto base    = "icon_default";
            auto blend   = texture_name;
            this->manager->texturizer.blendTextures(save_as, base, blend);
        }
    }
}

void Regionmap::update(float delta_time) {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();

    this->updateUI();
}

void Regionmap::render(float delta_time) {
    this->manager->window.clear(COLOUR_BLACK);    

    this->manager->window.getWindow()->setView(this->view_game);

    this->renderRegion();
    this->renderSelectedBuilding();
    this->higlightTile();

    this->manager->window.getWindow()->setView(this->view_interface);

    this->renderUI();

    this->manager->window.display();
}

void Regionmap::handleInput() {
    if(event_queue.size()) {
        const auto& event_name = event_queue.at(0);

        if(event_name == "WINDOW_RESIZE") {
            
        }

        if(event_name == "BUTTON_PRESSED") {
            if(this->controls.isKeyPressed("F3")) {
                this->toggleComponentVisibility("component_debug_performance");
            }

            if(this->controls.isKeyPressed("F12")) {
                this->manager->window.takeScreenshot();
            }

            if(this->controls.isKeyPressed("ESCAPE")) {
                this->manager->gamestate.setGamestate("worldmap");
            }

            if(this->controls.isKeyPressed("M")) {
                this->toggleComponentVisibility("component_minimap");
            }

            if(this->controls.isKeyPressed("I")) {
                this->toggleComponentVisibility("component_widget_region_storage");
                this->setVisibilityFalse("component_widget_building_menu");
            }
            
            if(this->controls.isKeyPressed("B")) {
                this->toggleComponentVisibility("component_widget_menu_building");
                this->setVisibilityFalse("component_widget_region_storage");

                auto building_menu = static_cast<gui::WidgetMenuBuilding*>(this->getInterfaceComponent("component_widget_menu_building"));
                building_menu->resetBuilding();
            }

            const auto& tile_top    = this->region->map[0];
            const auto& tile_left   = this->region->map[game_settings.calculateRegionIndex(0, game_settings.getRegionWidth() - 1)];
            const auto& tile_right  = this->region->map[game_settings.calculateRegionIndex(game_settings.getRegionWidth() - 1, 0)];
            const auto& tile_bottom = this->region->map[game_settings.calculateRegionIndex(game_settings.getRegionWidth() - 1, game_settings.getRegionWidth() - 1)];

            const auto bound_top    = tile_top.getPosition2D().y;
            const auto bound_left   = tile_left.getPosition2D().x;
            const auto bound_right  = tile_right.getPosition2D().x;
            const auto bound_bottom = tile_bottom.getPosition2D().y;

            if(this->controls.isKeyPressed("ARROW_LEFT"))
                if(this->view_game.getCenter().x - game_settings.tileSize().x >= bound_left)
                    this->view_game.move(-game_settings.tileSize().x, 0);

            if(this->controls.isKeyPressed("ARROW_RIGHT"))
                if(this->view_game.getCenter().x + game_settings.tileSize().x <= bound_right)
                    this->view_game.move(game_settings.tileSize().x, 0);

            if(this->controls.isKeyPressed("ARROW_DOWN"))
                if(this->view_game.getCenter().y + game_settings.tileSize().y <= bound_bottom)
                    this->view_game.move(0, game_settings.tileSize().y);

            if(this->controls.isKeyPressed("ARROW_UP"))
                if(this->view_game.getCenter().y - (game_settings.tileSize().y) >= bound_top)
                    this->view_game.move(0, -game_settings.tileSize().y);
        }

        if(event_name == "LMB_PRESSED") {
            this->updateTile();
        }

        if(event_name == "RMB_PRESSED") {

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

void Regionmap::moveCamera() {
    auto distance = sf::Vector2f(
        (this->controls.button_position_pressed.x - this->controls.button_position_released.x) / game_settings.tileSize().x,       
        (this->controls.button_position_pressed.y - this->controls.button_position_released.y) / game_settings.tileSize().y
    );

    // Multipliers for faster camera movement. 
    float x_multiplier = 6.0f;
    float y_multiplier = 6.0f;    

    auto top_tile    = this->region->map[0];
    auto left_tile   = this->region->map[game_settings.getRegionSize() - game_settings.getRegionWidth()];
    auto right_tile  = this->region->map[game_settings.getRegionWidth() - 1];
    auto bottom_tile = this->region->map[game_settings.getRegionSize() - 1];

    const int left_bound   = left_tile.getPosition().x;
    const int right_bound  = right_tile.getPosition().x + game_settings.tileSize().x;
    const int top_bound    = top_tile.getPosition().y;
    const int bottom_bound = bottom_tile.getPosition().y + game_settings.tileSize().y;

    if(this->view_game.getCenter().x + distance.x * x_multiplier > left_bound - this->view_game.getSize().x / 4 && this->view_game.getCenter().x + distance.x * x_multiplier < right_bound + this->view_game.getSize().x / 4)
        this->view_game.move(distance.x * x_multiplier, 0);

    if(this->view_game.getCenter().y + distance.y * y_multiplier > top_bound - this->view_game.getSize().y / 4 && this->view_game.getCenter().y + distance.y * y_multiplier < bottom_bound + this->view_game.getSize().y / 4)
        this->view_game.move(0, distance.y * y_multiplier);

    this->move_camera = false;
}

void Regionmap::zoomCamera() {
    // If you scroll up   - zoom in.
    // If you scroll down - zoom out.

    if((this->current_zoom > this->max_zoom_in && this->controls.mouseMiddleUp()) || (this->current_zoom < this->max_zoom_out && !this->controls.mouseMiddleUp())) {  
        if(this->controls.mouseMiddleUp()) {
            this->current_zoom = this->current_zoom - 1;
            this->view_game.zoom(0.5f);
        }
        
        else if(this->controls.mouseMiddleDown()) {
            this->current_zoom = this->current_zoom + 1;
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

sf::VertexBuffer regionmap_mesh_tile(sf::Quads, sf::VertexBuffer::Usage::Dynamic);
std::vector<GameObject> draw_order;
void Regionmap::renderRegion() {    
    const sf::Vector2f camera_size   = this->view_game.getSize();
    const sf::Vector2f camera_centre = this->view_game.getCenter();
    const sf::Rect     camera_screen_area(camera_centre - 0.5f * camera_size, camera_size);
    const sf::Vector2f tile_size = game_settings.tileSize();

    int gpu_draw_calls = 0;

    if(this->recalculate_mesh) {
        int verticies_index  = 0;
        int side_vector_size = 0;
        for(int i = 0; i < this->region->sides.size(); i++)
            side_vector_size += this->region->sides[i].size();

        const size_t verticies_tilemap = 4 * this->region->map.size() + 4 * side_vector_size;

        sf::Vertex verticies_tiles[verticies_tilemap];

        if(!regionmap_mesh_tile.getVertexCount())
            regionmap_mesh_tile.create(verticies_tilemap);

        for(int index = 0; index < game_settings.getRegionSize(); index++) {
            sf::Vertex* quad = &verticies_tiles[verticies_index * 4];
            
            auto& tile          = this->region->map.at(index);
            auto tile_position  = tile.getPosition2D();
            const auto& texture = tile.getTextureName();

            quad[0].position = tile_position + sf::Vector2f(0, 0);
            quad[1].position = tile_position + sf::Vector2f(tile_size.x, 0);
            quad[2].position = tile_position + sf::Vector2f(tile_size.x, tile_size.y);
            quad[3].position = tile_position + sf::Vector2f(0, tile_size.y);

            const auto tile_coords = this->manager->resource.getTexturePosition(texture); 

            quad[0].texCoords = tile_coords + sf::Vector2f(0, 0);
            quad[1].texCoords = tile_coords + sf::Vector2f(tile_size.x, 0);
            quad[2].texCoords = tile_coords + sf::Vector2f(tile_size.x, tile_size.y);
            quad[3].texCoords = tile_coords + sf::Vector2f(0, tile_size.y);

            // Add one to the index because you add the tile.
            verticies_index++;  

            for(int side_index = 0; side_index < this->region->sides[index].size(); side_index++) {
                sf::Vertex* side_quad = &verticies_tiles[verticies_index * 4];

                const auto& side         = this->region->sides[index][side_index];
                const auto side_position = side.getPosition2D();

                side_quad[0].position = side_position + sf::Vector2f(0, 0);
                side_quad[1].position = side_position + sf::Vector2f(tile_size.x, 0);
                side_quad[2].position = side_position + sf::Vector2f(tile_size.x, tile_size.y);
                side_quad[3].position = side_position + sf::Vector2f(0, tile_size.y);

                const auto& side_texture = side.getTextureName();
                const auto side_coords   = this->manager->resource.getTexturePosition(side_texture);

                side_quad[0].texCoords = side_coords + sf::Vector2f(0, 0);
                side_quad[1].texCoords = side_coords + sf::Vector2f(tile_size.x, 0);
                side_quad[2].texCoords = side_coords + sf::Vector2f(tile_size.x, tile_size.y);
                side_quad[3].texCoords = side_coords + sf::Vector2f(0, tile_size.y);

                // Add one to the index because you add the side.
                verticies_index++;
            }
        }

        regionmap_mesh_tile.update(verticies_tiles);
        this->recalculate_mesh = false;
    }

    if(this->recalculate_tree_mesh) {
        draw_order = std::vector<GameObject>();

        for(int i = 0; i < game_settings.getRegionSize(); i++) {
            if(this->region->trees.count(i))
                draw_order.push_back(this->region->trees[i]);
            
            if(this->region->buildingExistsAt(i)) {
                if(this->region->buildings[i] != BUILDING_EMPTY)
                    draw_order.push_back(this->region->getBuildingAt(i));
            }
        }

        // for(int i = 0; i < this->region->getPopulation(); i++) {
        //     draw_order.push_back(this->region->population[i]);
        // }

        const auto compare = [](const GameObject& o1, const GameObject& o2) {
            auto o1_y = o1.getPosition2D().y + o1.getSize().y;
            auto o2_y = o2.getPosition2D().y + o2.getSize().y;

            if(o1_y < o2_y)
                return true;

            return false;
        };

        std::sort(draw_order.begin(), draw_order.end(), compare);
        this->recalculate_tree_mesh = false;
    }

    sf::RenderStates states_tiles;
    states_tiles.texture = &this->manager->resource.getTexture("tile_atlas"); 
    this->manager->window.draw(regionmap_mesh_tile, states_tiles);
    gpu_draw_calls++;

    for(const auto& object : draw_order) {
        sf::RenderStates states;
        states.texture = &this->manager->resource.getTexture(object.getTextureName());

        if(!this->manager->inScreenSpace(object))
            continue;

        sf::VertexArray game_object(sf::Quads, 4);
        auto position2d = object.getPosition2D();
        auto size       = object.getSize();

        game_object[0].position = position2d;    
        game_object[1].position = position2d + sf::Vector2f(size.x, 0);
        game_object[2].position = position2d + sf::Vector2f(size.x, size.y);
        game_object[3].position = position2d + sf::Vector2f(0, size.y);

        game_object[0].texCoords = sf::Vector2f(0, 0);
        game_object[1].texCoords = sf::Vector2f(size.x, 0);
        game_object[2].texCoords = sf::Vector2f(size.x, size.y);
        game_object[3].texCoords = sf::Vector2f(0, size.y);
        
        this->manager->window.draw(game_object, states);
        gpu_draw_calls++;
    }

    this->manager->updateDrawCalls(gpu_draw_calls);
}

void Regionmap::higlightTile() {
    auto tile_size   = game_settings.tileSize();
    auto tile_offset = game_settings.tileOffset();
    auto region_size = game_settings.getRegionSize();

    sf::Vector2i mouse_position(
        this->mouse_position_window.x,
        this->mouse_position_window.y
    );

    sf::Vector2i cell(
        mouse_position.x / tile_size.x,
        mouse_position.y / tile_size.y
    );

    // Offset withing the tile.
    // Used to find pixel colour on the cheat-texture.
    sf::Vector2i offset(
        (int)mouse_position.x % (int)tile_size.x,
        (int)mouse_position.y % (int)tile_size.y
    );

    sf::Vector2i selected(
        (cell.y - tile_offset.y) + (cell.x - tile_offset.x),
        (cell.y - tile_offset.y) - (cell.x - tile_offset.x)
    );

    auto colour_name = getTilePixelColour(this->manager->resource.getTexture("tile_template_direction") ,offset);
    if(colour_name == "Red")
        selected += sf::Vector2i(-1, 0);

    if(colour_name == "Green")
        selected += sf::Vector2i(1, 0);

    if(colour_name == "Blue")
        selected += sf::Vector2i(0, -1);
    
    if(colour_name == "Yellow")
        selected += sf::Vector2i(1, 0);

    if(!game_settings.inRegionBounds(selected))
        return;

    int index = game_settings.calculateRegionIndex(selected.x, selected.y);
    this->current_index = index;
    if(this->mouse_drag)
        return;

    if(!this->controls.mouseRightPressed())
        return;

    auto building_menu = static_cast<gui::WidgetMenuBuilding*>(this->getInterfaceComponent("component_widget_menu_building"));
    if(building_menu->getBuilding() != BUILDING_EMPTY)
        return;

    const auto& tile   = this->region->map[index];
    auto tile_position = tile.getPosition2D();

    sf::VertexArray highlight(sf::Quads, 4);

    highlight[0].position = tile_position; 
    highlight[1].position = tile_position + sf::Vector2f(tile_size.x, 0);
    highlight[2].position = tile_position + sf::Vector2f(tile_size.x, tile_size.y);
    highlight[3].position = tile_position + sf::Vector2f(0, tile_size.y);

    highlight[0].texCoords = sf::Vector2f(0, 0);
    highlight[1].texCoords = sf::Vector2f(tile_size.x, 0);
    highlight[2].texCoords = sf::Vector2f(tile_size.x, tile_size.y);
    highlight[3].texCoords = sf::Vector2f(0, tile_size.y);

    sf::RenderStates states;
    states.texture = &this->manager->resource.getTexture("tile_highlight_1x1");
    this->manager->window.draw(highlight, states);

    const auto& current_tile = this->region->map[this->current_index];
    
    std::string tile_data;
    tile_data += this->region->biome.getBiomeName() + " " + current_tile.getName() + " | #" + std::to_string(this->current_index) + "\n";

    if(this->region->resourceExistsAt(this->current_index))
        tile_data += this->region->getResourceAt(this->current_index).getResourceName() + "\n";

    auto cursor_offset = sf::Vector2f(
        game_settings.tileSize().x / 2,
        game_settings.tileSize().y / 2
    );

    gui::Label tile_information(this->manager, tile_data);
    tile_information.setWidgetPosition(this->mouse_position_interface + cursor_offset);

    auto current_view = this->manager->window.getWindow()->getView();
     
    this->manager->window.getWindow()->setView(this->view_interface);
    this->manager->window.draw(tile_information);
    this->manager->window.getWindow()->setView(current_view);
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
    auto* building_menu = static_cast<gui::WidgetMenuBuilding*>(this->getInterfaceComponent("component_widget_menu_building"));
    if(building_menu->getBuilding() != BUILDING_EMPTY && this->controls.mouseLeftPressed() && building_menu->isVisible() && !this->mouseIntersectsUI() && !this->mouse_drag) {
        auto building = building_menu->getBuilding();
        auto grid_position = tileGridPosition(this->current_index);
        auto* human = this->manager->getHumanPlayer();
        
        human->placeBuildingCheck(*this->region, building, grid_position);
        this->updatePaths(this->current_index);
        this->recalculate_tree_mesh = true;
    }
}

void Regionmap::updatePaths(int index) {
    /*
    if(!this->region->getBuildingAt(index) || *this->region->getBuildingAt(index) == BUILDING_EMPTY)
        return;

    // Assigns appropriate path texture based on the path tile's surroundings.
    // Only update the paths if they are adjacent to other paths.
    const auto directions = [this](bool LEFT, bool RIGHT, bool TOP, bool DOWN, int index) -> void {
        auto building = this->region->getBuildingAt(index);
        const std::string path_type = (*building == BUILDING_PATH_DIRT) ? "dirt" : "stone"; 

        if(LEFT && RIGHT && !TOP && !DOWN)
            building->object_texture_name = "path_" + path_type + "_horizontal";

        else if(!LEFT && !RIGHT && TOP && DOWN)
            building->object_texture_name = "path_" + path_type + "_vertical";

        else if(LEFT && RIGHT && TOP && DOWN)
            building->object_texture_name = "path_" + path_type + "_cross";

        else if(LEFT && !RIGHT && TOP && !DOWN)
            building->object_texture_name = "path_" + path_type + "_turn_up";
    
        else if(!LEFT && RIGHT && !TOP && DOWN)
            building->object_texture_name = "path_" + path_type + "_turn_down";

        else if(!LEFT && RIGHT && TOP && !DOWN)
            building->object_texture_name = "path_" + path_type + "_turn_right";

        else if(LEFT && !RIGHT && !TOP && DOWN)
            building->object_texture_name = "path_" + path_type + "_turn_left";

        else if(LEFT && RIGHT && TOP && !DOWN)
            building->object_texture_name = "path_" + path_type + "_without_down";

        else if(LEFT && RIGHT && !TOP && DOWN)
            building->object_texture_name = "path_" + path_type + "_without_top";

        else if(LEFT && !RIGHT && TOP && DOWN)
            building->object_texture_name = "path_" + path_type + "_without_right";

        else if(!LEFT && RIGHT && TOP && DOWN)
            building->object_texture_name = "path_" + path_type + "_without_left";
    
        else if((LEFT && !RIGHT && !TOP && !DOWN) || (!LEFT && RIGHT && !TOP && !DOWN))
            building->object_texture_name = "path_" + path_type + "_horizontal";

        else if((!LEFT && !RIGHT && !TOP && DOWN) || (!LEFT && !RIGHT && TOP && !DOWN))
            building->object_texture_name = "path_" + path_type + "_vertical";
        
        else building->object_texture_name = "path_" + path_type + "_point";
    };

    // Update the singular tile.
    auto building_at_index = this->region->getBuildingAt(index);
    
    // Check if the pointer exists.
    // It might be a nullptr, and you do not want to derefence that.
    if(building_at_index) {
        if(*building_at_index == BUILDING_PATH_DIRT || *building_at_index == BUILDING_PATH_STONE) {
            bool LEFT  = this->region->isPath(index - 1);
            bool RIGHT = this->region->isPath(index + 1);
            bool TOP   = this->region->isPath(index - game_settings.getRegionWidth());
            bool DOWN  = this->region->isPath(index + game_settings.getRegionWidth());
            
            directions(LEFT, RIGHT, TOP, DOWN, index);
        }
    }

    // Update the tiles that might have been affected by the previous change.
    for(auto& pair : this->region->buildings) {
        int i = pair.first;

        if(this->region->isPath(i)) {
            bool LEFT  = this->region->isPath(i - 1);
            bool RIGHT = this->region->isPath(i + 1);
            bool TOP   = this->region->isPath(i - game_settings.getRegionWidth());
            bool DOWN  = this->region->isPath(i + game_settings.getRegionWidth());
            
            directions(LEFT, RIGHT, TOP, DOWN, i);
        }
    }
    */
}

void Regionmap::createUI() {
    auto widget_menu_building = gui::InterfaceComponent(new gui::WidgetMenuBuilding(this->manager));
    auto widget_performance   = gui::InterfaceComponent(new gui::DebugPerformance(this->manager));
    auto widget_minimap       = gui::InterfaceComponent(new gui::WidgetMinimap(this->manager));
    auto widget_storage       = gui::InterfaceComponent(new gui::WidgetRegionStorage(this->manager));
    auto widget_tooltip       = gui::InterfaceComponent(new gui::Tooltip(this->manager));
    
    this->addInterfaceComponent(widget_menu_building);
    this->addInterfaceComponent(widget_performance);
    this->addInterfaceComponent(widget_minimap);
    this->addInterfaceComponent(widget_storage);
    this->addInterfaceComponent(widget_tooltip);
}

void Regionmap::updateScheduler() {    
    // if(game_settings.astarEnabled()) {
    //     auto& update_population_path = this->scheduler.at("update_path");
    //     if(update_population_path.first != update_population_path.second)
    //         update_population_path.first++;

    //     if(update_population_path.first == update_population_path.second) {
    //         for(auto& unit : this->region->population) {
    //             if(!unit.hasPath()) {
    //                 auto random_index = rand() % game_settings.getRegionSize();
    //                 auto found = false;
    //                 while(!found) {
    //                     random_index = rand() % game_settings.getRegionSize();
    //                     if(this->region->map[random_index].tiletype.is_terrain() && !this->region->isTree(random_index) && !this->region->getBuildingAt(region_index)) {
    //                         found = true;
    //                         break;
    //                     }
    //                 } 

    //                 auto path = this->manager->r_astar(unit.current_index, random_index);
    //                 unit.setNewPath(path);
                    
    //                 this->recalculate_mesh = true;
    //             }
    //         }

    //         update_population_path.first = 0;
    //     }

    //     auto& update_population_movement = this->scheduler.at("update_movement");
    //     if(update_population_movement.first != update_population_movement.second)
    //         update_population_movement.first++;
        
    //     if(update_population_movement.first == update_population_movement.second) {
    //         for(auto& unit : this->region->population) {
    //             if(unit.hasPath()) {
    //                 auto current = unit.current_index;
    //                 auto next    = unit.getNextMove();
                
    //                 const auto& tile  = this->region->map[next];
    //                 auto new_position = tile.getPosition();

    //                 unit.object_position = new_position + sf::Vector3f(0, -(unit.object_size.y - game_settings.tileSize().y), 0);
    //                 unit.current_index   = next;

    //                 // Assign texture.
                    
    //                 std::string texture_base = tile.tiletype.is_river()
    //                     ? "unit_transport_arrow"
    //                     : "unit_classic_arrow";

    //                 auto index_difference = next - current;
                    
    //                 if(index_difference == 1)
    //                     unit.object_texture_name = texture_base + "_br";

    //                 else if(index_difference == -1)
    //                     unit.object_texture_name = texture_base + "_tl";

    //                 else if(index_difference == game_settings.getRegionWidth())
    //                     unit.object_texture_name = texture_base + "_bl";

    //                 else if(index_difference == -game_settings.getRegionWidth())
    //                     unit.object_texture_name = texture_base + "_tr";
    //             }
    //         }
            
    //         this->recalculate_tree_mesh = true;
    //         update_population_movement.first = 0;
    //     }
    // }
}

void Regionmap::gamestateLoad() {
    // Region's index was set upon entering the region.
    // It's set in widgetRegion.cpp
    this->region = &this->manager->world.world_map[this->region_index];

    this->recalculate_mesh      = true;
    this->recalculate_tree_mesh = true;

    int side_vector_size = 0;
    for(int i = 0; i < this->region->sides.size(); i++)
        side_vector_size += this->region->sides[i].size();

    const size_t verticies_tilemap = 4 * this->region->map.size() + 4 * side_vector_size;

    regionmap_mesh_tile.create(verticies_tilemap);

    this->manager->window.getWindow()->setView(this->view_interface);
    this->resizeViews();
    this->resizeUI();
    this->manager->window.getWindow()->setView(this->view_game);

    // Tile index on which you centre the camera.
    auto tile_index = game_settings.calculateRegionIndex(game_settings.getRegionWidth() / 2, game_settings.getRegionWidth() / 2);

    // Here you can setup what to do when entering a region.
    // For example, centre the camera.
    sf::Vector2f first_tile_position = sf::Vector2f(
        this->region->map[tile_index].getPosition().x + game_settings.tileSize().x / 2,
        this->region->map[tile_index].getPosition().y
    );

    // if(this->region->isOwned() && this->region->getPopulation() == 0) {
    //     this->region->population.resize(game_settings.getRegionInitialPopulation());

    //     std::vector <int> buffer;
    //     std::vector <std::string> directions = { "_tl", "_tr", "_bl", "_br" };
    //     for(int i = 0; i < game_settings.getRegionInitialPopulation(); i++) {
    //         auto& pop = this->region->population[i];
    //         auto  free_spot = this->region->findNotOccupiedTile(buffer);
    //         pop.current_index = free_spot;

    //         const auto& tile = this->region->map[free_spot];
    //         const auto tile_position = tile.getPosition();
    //         std::string base_texture = tile.tiletype.is_river() 
    //             ? "unit_transport_arrow"
    //             : "unit_classic_arrow";

    //         pop.object_size         = sf::Vector2f(64, 96);
    //         pop.object_position     = tile_position + sf::Vector3f(0, -(pop.object_size.y - game_settings.tileSize().y), 0);
    //         pop.object_texture_name = base_texture + directions[rand() % directions.size()];

    //         buffer.push_back(free_spot);
    //     }
    // }

    this->view_game.setCenter(first_tile_position);
    
    auto minimap = static_cast<gui::WidgetMinimap*>(this->getInterfaceComponent("component_minimap"));
        minimap->should_redraw = true;

    this->region->visited = true;

    for(auto& pair : this->interface) {
        auto* component = pair.second.get();
        this->setVisibilityFalse(component->getWidgetID());
    }
}

void Regionmap::gamestateClose() {
    this->recalculate_mesh      = false;
    this->recalculate_tree_mesh = false;
    regionmap_mesh_tile.create(0);
}

void Regionmap::recalculateMesh() {
    this->recalculate_mesh      = true;
    this->recalculate_tree_mesh = true;
}

void Regionmap::renderSelectedBuilding() {
    auto* building_menu = static_cast<gui::WidgetMenuBuilding*>(this->getInterfaceComponent("component_widget_menu_building"));
    auto  building      = building_menu->getBuilding();
    if(building != BUILDING_EMPTY && building_menu->isVisible()) {
        auto tile = this->region->map[this->current_index];

        auto building_size = building.getBuildingArea(); 
        auto grid_position = tileGridPosition(this->current_index);
        
        for(int y = grid_position.y; y < grid_position.y + building_size.y; y++) {
            for(int x = grid_position.x; x < grid_position.x + building_size.x; x++) {
                auto grid = sf::Vector2i(x, y);
                if(!game_settings.inRegionBounds(grid)) 
                    return;
            }
        }
        
        building.object_position = tile.getPosition();
        
        const int a1_w = 0; 
        const int a1_h = game_settings.tileSize().y; 
        const int r_w  = game_settings.tileSize().x / 2;
        const int r_h  = game_settings.tileSize().y;    
        
        // This takes only one dimension into account because this engine currently
        // does not support non-square buildings.
        const int n = building.getBuildingArea().x;                    

        // Here you adjust the origin of buildings with sizes of n > 0.
        // Texture size scale are arithmetic series.
        auto offset = sf::Vector2f(0, 0);  
        if(n > 0) {
            offset = sf::Vector2f(
                -(a1_w + (n - 1) * r_w),
                -(a1_h + (n - 1) * r_h)
            );
        }

        // First draw the border of the building.
        // Afterwards, draw the building highlight itself.
        // Border.
        
        auto building_area = building.getBuildingArea().x;
        auto tile_size     = game_settings.tileSize(); 
        int  highlight_size_x = building_area * tile_size.x;
        int  highlight_size_y = building_area * tile_size.y;

        auto top    = building.getPosition2D() + sf::Vector2f(-(building_area - 1) * tile_size.x / 2, 0) + sf::Vector2f(highlight_size_x / 2, 0);
        auto right  = building.getPosition2D() + sf::Vector2f(-(building_area - 1) * tile_size.x / 2, 0) + sf::Vector2f(highlight_size_x, highlight_size_y / 2);
        auto left   = building.getPosition2D() + sf::Vector2f(-(building_area - 1) * tile_size.x / 2, 0) + sf::Vector2f(0, highlight_size_y / 2);
        auto bottom = building.getPosition2D() + sf::Vector2f(-(building_area - 1) * tile_size.x / 2, 0) + sf::Vector2f(highlight_size_x / 2, highlight_size_y);  

        sf::VertexArray building_surround_highlight(sf::Lines, 8);

        building_surround_highlight[0].position = top; 
        building_surround_highlight[1].position = right;
        building_surround_highlight[2].position = right;
        building_surround_highlight[3].position = bottom;
        building_surround_highlight[4].position = bottom;
        building_surround_highlight[5].position = left;
        building_surround_highlight[6].position = left;
        building_surround_highlight[7].position = top;

        building_surround_highlight[0].color = COLOUR_BLUE_RIVER;
        building_surround_highlight[1].color = COLOUR_BLUE_RIVER;
        building_surround_highlight[2].color = COLOUR_BLUE_RIVER;
        building_surround_highlight[3].color = COLOUR_BLUE_RIVER;
        building_surround_highlight[4].color = COLOUR_BLUE_RIVER;
        building_surround_highlight[5].color = COLOUR_BLUE_RIVER;
        building_surround_highlight[6].color = COLOUR_BLUE_RIVER;
        building_surround_highlight[7].color = COLOUR_BLUE_RIVER;

        this->manager->window.draw(building_surround_highlight);

        // Highlight.

        sf::VertexArray building_highlight(sf::Quads, 4);

        building_highlight[0].position = building.getPosition2D() + offset;
        building_highlight[1].position = building.getPosition2D() + offset + sf::Vector2f(building.getSize().x, 0);
        building_highlight[2].position = building.getPosition2D() + offset + sf::Vector2f(building.getSize().x, building.getSize().y); 
        building_highlight[3].position = building.getPosition2D() + offset + sf::Vector2f(0, building.getSize().y);

        building_highlight[0].texCoords = sf::Vector2f(0, 0);
        building_highlight[1].texCoords = sf::Vector2f(building.getSize().x, 0); 
        building_highlight[2].texCoords = sf::Vector2f(building.getSize().x, building.getSize().y); 
        building_highlight[3].texCoords = sf::Vector2f(0, building.getSize().y);

        auto invalid_position = !this->region->isBuildingPositionValid(building, grid_position); 
        if(invalid_position) {
            building_highlight[0].color = COLOUR_RED;
            building_highlight[1].color = COLOUR_RED;
            building_highlight[2].color = COLOUR_RED;
            building_highlight[3].color = COLOUR_RED;
        }

        const auto texture = building.getTextureName();
        
        sf::RenderStates states;
        states.texture = &this->manager->resource.getTexture(texture);
        this->manager->window.draw(building_highlight, states);

        const int area_x = building.getBuildingScanArea().x;
        const int area_y = building.getBuildingScanArea().y;
        
        if(area_x > 1 && area_y > 1) {
            int vertex_index = 0; 
            const size_t vertex_count = 4 * (2 * building.getBuildingScanArea().x + building_area) * (2 * building.getBuildingScanArea().y + building_area);
            sf::Vertex vertex[vertex_count];
            for(int y = -area_y; y <= area_y + building_area - 1; y++) {
                for(int x = -area_x; x <= area_x + building_area - 1; x++) {
                    const int index = this->current_index + game_settings.calculateRegionIndex(x, y);

                    if(!game_settings.inRegionBounds(index))
                        continue;

                    auto& tile = this->region->map[index];
                    auto tile_position = tile.getPosition2D();
                    auto tile_size     = tile.getSize();

                    sf::Vertex* quad = &vertex[vertex_index * 4];

                    quad[0].position = tile_position + sf::Vector2f(0, 0); 
                    quad[1].position = tile_position + sf::Vector2f(tile_size.x, 0); 
                    quad[2].position = tile_position + sf::Vector2f(tile_size.x, tile_size.y); 
                    quad[3].position = tile_position + sf::Vector2f(0, tile_size.y); 

                    quad[0].texCoords = sf::Vector2f(0, 0);
                    quad[1].texCoords = sf::Vector2f(tile_size.x, 0);
                    quad[2].texCoords = sf::Vector2f(tile_size.x, tile_size.y);
                    quad[3].texCoords = sf::Vector2f(0, tile_size.y);

                    auto colour = COLOUR_WHITE_TRANSPARENT90PERCENT;
                    if(building.isTileHarvestable(this->region, index))
                        colour = COLOUR_GREEN_TRANSPARENT90PERCENT;

                    else if(region->isSpotOccupied(index))
                        colour = COLOUR_RED_TRANSPARENT90PERCENT;

                    quad[0].color = colour;
                    quad[1].color = colour;
                    quad[2].color = colour;
                    quad[3].color = colour;

                    vertex_index++;
                }
            }

            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture("tile_transparent_white");
            this->manager->window.getWindow()->draw(vertex, vertex_count, sf::Quads, states);
        }
    }
}