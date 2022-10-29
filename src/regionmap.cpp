#include "regionmap.hpp"
#include "worldData.hpp"
#include "globalutilities.hpp"
#include "simulationManager.hpp"
#include "colours.hpp"
#include "worldmap.hpp"

#include <filesystem>

namespace iso {
Regionmap::Regionmap() : Gamestate("Regionmap") 
    , tile_index(-1), region_index(-1), region(nullptr), recalculate_tile_mesh(false), recalculate_tree_mesh(false)
{
    this->initialise();
    this->loadResources();
}

Regionmap::~Regionmap() 
{}

void Regionmap::initialise() {
    this->default_zoom  = 0;
    this->current_zoom  = this->default_zoom;
    this->max_zoom_in   = 0; 
    this->max_zoom_out  = 3;
    
    this->view_interface.setCenter(game_manager.window.getWindowWidth() / 2, game_manager.window.getWindowHeight() / 2);
    this->view_game.setCenter(game_manager.window.getWindowWidth() / 2, game_manager.window.getWindowHeight() / 2);
}

void Regionmap::loadResources() {
    game_manager.resource.paintTexture("template_highlight_1x1", "tile_highlight_1x1"    , COLOUR_WHITE_TRANSPARENT_HALF, COLOUR_TRANSPARENT);
    game_manager.resource.paintTexture("template_highlight_1x1", "tile_transparent_white", COLOUR_WHITE_TRANSPARENT_HALF, COLOUR_TRANSPARENT);

    // Automatically generate icon names.
    for(const auto& texture_pair : game_manager.resource.getTextureCollection()) {
        const auto& texture_name = texture_pair.first;

        // Generate icons.
        if(startsWith(texture_name, "icon_template_")) {
            auto save_as = "icon_" + readAfter(texture_name, "icon_template_");
            auto base    = "icon_default";
            auto blend   = texture_name;
            game_manager.resource.blendTextures(save_as, base, blend);
        }
    }
}

void Regionmap::update(float delta_time) {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();
    this->redrawUI();
}

void Regionmap::render(float delta_time) {
    game_manager.window.clear(COLOUR_BLACK);    

    game_manager.window.setView(this->view_game);

    this->renderRegion();
    this->higlightTile();

    game_manager.window.setView(this->view_interface);

    this->renderUI();

    game_manager.window.display();
}

void Regionmap::handleInput() {
    if(event_queue.size()) {
        const auto& event_name = event_queue.at(0);

        if(event_name == "WINDOW_RESIZE") {
            this->resizeUI();
            this->resizeViews();
        }

        if(event_name == "BUTTON_PRESSED" && !this->controls.isInputBlocked()) {
            if(this->controls.isKeyPressed("F12")) {
                game_manager.window.takeScreenshot();
            }

            if(this->controls.isKeyPressed("ESCAPE")) {
                game_manager.gamestate.setGamestate("Worldmap");
            }

            const auto& tile_top    = this->region->map[0];
            const auto& tile_left   = this->region->map[calculateRegionIndex(0, world_data.r_width - 1)];
            const auto& tile_right  = this->region->map[calculateRegionIndex(world_data.r_width - 1, 0)];
            const auto& tile_bottom = this->region->map[calculateRegionIndex(world_data.r_width - 1, world_data.r_width - 1)];

            const auto bound_top    = tile_top.getPosition2D().y;
            const auto bound_left   = tile_left.getPosition2D().x;
            const auto bound_right  = tile_right.getPosition2D().x;
            const auto bound_bottom = tile_bottom.getPosition2D().y;

            if(this->controls.isKeyPressed("ARROW_LEFT"))
                if(this->view_game.getCenter().x - tileSize().x >= bound_left)
                    this->view_game.move(-tileSize().x, 0);

            if(this->controls.isKeyPressed("ARROW_RIGHT"))
                if(this->view_game.getCenter().x + tileSize().x <= bound_right)
                    this->view_game.move(tileSize().x, 0);

            if(this->controls.isKeyPressed("ARROW_DOWN"))
                if(this->view_game.getCenter().y + tileSize().y <= bound_bottom)
                    this->view_game.move(0, tileSize().y);

            if(this->controls.isKeyPressed("ARROW_UP"))
                if(this->view_game.getCenter().y - (tileSize().y) >= bound_top)
                    this->view_game.move(0, -tileSize().y);
        }

        if(event_name == "LMB_PRESSED") {
        
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
        (this->controls.button_position_pressed.x - this->controls.button_position_released.x) / tileSize().x,       
        (this->controls.button_position_pressed.y - this->controls.button_position_released.y) / tileSize().y
    );

    // Multipliers for faster camera movement. 
    float x_multiplier = 6.0f;
    float y_multiplier = 6.0f;    

    auto top_tile    = this->region->map[0];
    auto left_tile   = this->region->map[getRegionSize() - world_data.r_width];
    auto right_tile  = this->region->map[world_data.r_width - 1];
    auto bottom_tile = this->region->map[getRegionSize() - 1];

    const int left_bound   = left_tile.getPosition().x;
    const int right_bound  = right_tile.getPosition().x + tileSize().x;
    const int top_bound    = top_tile.getPosition().y;
    const int bottom_bound = bottom_tile.getPosition().y + tileSize().y;

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
    const sf::Vector2f tile_size = tileSize().asSFMLVector2f();

    if(this->recalculate_tile_mesh) {
        int verticies_index  = 0;
        int side_vector_size = 0;
        for(int i = 0; i < this->region->sides.size(); i++)
            side_vector_size += this->region->sides[i].size();

        const size_t verticies_tilemap = 4 * this->region->map.size() + 4 * side_vector_size;

        sf::Vertex verticies_tiles[verticies_tilemap];

        if(!regionmap_mesh_tile.getVertexCount())
            regionmap_mesh_tile.create(verticies_tilemap);

        for(int index = 0; index < getRegionSize(); index++) {
            sf::Vertex* quad = &verticies_tiles[verticies_index * 4];
            
            auto& tile          = this->region->map.at(index);
            auto tile_position  = tile.getPosition2D().asSFMLVector2f();
            const auto& texture = tile.getTextureName();

            quad[0].position = tile_position + sf::Vector2f(0, 0);
            quad[1].position = tile_position + sf::Vector2f(tile_size.x, 0);
            quad[2].position = tile_position + sf::Vector2f(tile_size.x, tile_size.y);
            quad[3].position = tile_position + sf::Vector2f(0, tile_size.y);

            const auto tile_coords = game_manager.resource.getTexturePosition(texture).asSFMLVector2f(); 

            quad[0].texCoords = tile_coords + sf::Vector2f(0, 0);
            quad[1].texCoords = tile_coords + sf::Vector2f(tile_size.x, 0);
            quad[2].texCoords = tile_coords + sf::Vector2f(tile_size.x, tile_size.y);
            quad[3].texCoords = tile_coords + sf::Vector2f(0, tile_size.y);

            // Add one to the index because you add the tile.
            verticies_index++;  

            for(int side_index = 0; side_index < this->region->sides[index].size(); side_index++) {
                sf::Vertex* side_quad = &verticies_tiles[verticies_index * 4];

                const auto& side         = this->region->sides[index][side_index];
                const auto side_position = side.getPosition2D().asSFMLVector2f();

                side_quad[0].position = side_position + sf::Vector2f(0, 0);
                side_quad[1].position = side_position + sf::Vector2f(tile_size.x, 0);
                side_quad[2].position = side_position + sf::Vector2f(tile_size.x, tile_size.y);
                side_quad[3].position = side_position + sf::Vector2f(0, tile_size.y);

                const auto& side_texture = side.getTextureName();
                const auto side_coords   = game_manager.resource.getTexturePosition(side_texture).asSFMLVector2f();

                side_quad[0].texCoords = side_coords + sf::Vector2f(0, 0);
                side_quad[1].texCoords = side_coords + sf::Vector2f(tile_size.x, 0);
                side_quad[2].texCoords = side_coords + sf::Vector2f(tile_size.x, tile_size.y);
                side_quad[3].texCoords = side_coords + sf::Vector2f(0, tile_size.y);

                // Add one to the index because you add the side.
                verticies_index++;
            }
        }

        regionmap_mesh_tile.update(verticies_tiles);
        this->recalculate_tile_mesh = false;
    }

    if(this->recalculate_tree_mesh) {
        draw_order = std::vector<GameObject>();

        for(int i = 0; i < getRegionSize(); i++) {
            if(this->region->trees.count(i))
                draw_order.push_back(this->region->trees[i]);
            
            // if(this->region->buildingExistsAtIndex(i)) {
            //     if(this->region->buildings[i] != BUILDING_EMPTY)
            //         draw_order.push_back(this->region->getBuildingAtIndex(i));
            // }
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
    states_tiles.texture = &game_manager.resource.getTexture("tile_atlas"); 
    game_manager.window.draw(regionmap_mesh_tile, states_tiles);

    for(const auto& object : draw_order) {
        sf::RenderStates states;
        states.texture = &game_manager.resource.getTexture(object.getTextureName());

        if(!game_manager.window.inScreenSpace(object))
            continue;

        sf::VertexArray game_object(sf::Quads, 4);
        auto position2d = object.getPosition2D().asSFMLVector2f();
        auto size       = object.getSize().asSFMLVector2f();

        game_object[0].position = position2d;    
        game_object[1].position = position2d + sf::Vector2f(size.x, 0);
        game_object[2].position = position2d + sf::Vector2f(size.x, size.y);
        game_object[3].position = position2d + sf::Vector2f(0, size.y);

        game_object[0].texCoords = sf::Vector2f(0, 0);
        game_object[1].texCoords = sf::Vector2f(size.x, 0);
        game_object[2].texCoords = sf::Vector2f(size.x, size.y);
        game_object[3].texCoords = sf::Vector2f(0, size.y);
        
        game_manager.window.draw(game_object, states);
    }

    for(const auto& pair : this->region->buildings) {
        sf::RenderStates states;
        states.texture = &game_manager.resource.getTexture(pair.second.getTextureName());
        game_manager.window.draw(pair.second, states);
    }
}

void Regionmap::higlightTile() {
    auto tile_size = tileSize().asSFMLVector2f();

    auto mouse_position = core::Vector2f(
        this->mouse_position_window.x,
        this->mouse_position_window.y
    );

    auto selected = tileGridPosition(mouse_position);
    if(!inRegionBounds(selected)) {
        this->tile_index = -1;
        return;
    }

    int index = calculateRegionIndex(selected.x, selected.y);
    this->tile_index = index;
    
    if(this->controls.mouse_dragged)
        return;

    if(!this->controls.mouseRightPressed())
        return;

    const auto& tile   = this->region->map[index];
    auto tile_position = tile.getPosition2D().asSFMLVector2f();

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
    states.texture = &game_manager.resource.getTexture("tile_highlight_1x1");
    game_manager.window.draw(highlight, states);
}

int Regionmap::L_getRegionIndex() const {
    return this->region_index;
}

void Regionmap::L_setRegionIndex(int index) {
    if(!inWorldBounds(index)) {
        printError("Regionmap::L_setRegionIndex()", "Region index out of bounds");
        return;
    }

    this->region_index = index;
    this->region = &game_manager.world_map.at(index);
}

int Regionmap::L_getTileIndex() const {
    return this->tile_index;
}

void Regionmap::gamestateLoad() {
    if(!inWorldBounds(this->region_index)) {
        printError("Regionmap::gamestateLoad()", "Requested load of region out of world bounds");
        return;
    }

    this->region = &game_manager.world_map.at(this->region_index);

    if(!this->region->visited)
        game_manager.world_generator.generateRegion(this->region_index);

    this->recalculateMesh();

    game_manager.window.setView(this->view_interface);
    this->resizeViews();
    this->resizeUI();
    game_manager.window.setView(this->view_game);

    int side_vector_size = 0;
    for(int i = 0; i < this->region->sides.size(); i++)
        side_vector_size += this->region->sides[i].size();

    const size_t verticies_tilemap = 4 * this->region->map.size() + 4 * side_vector_size;

    regionmap_mesh_tile.create(verticies_tilemap);

    // Tile index on which you centre the camera.
    auto tile_index = calculateRegionIndex(world_data.r_width / 2, world_data.r_width / 2);

    // Here you can setup what to do when entering a region.
    // For example, centre the camera.
    sf::Vector2f first_tile_position = sf::Vector2f(
        this->region->map[tile_index].getPosition().x + tileSize().x / 2,
        this->region->map[tile_index].getPosition().y
    );

    this->view_game.setCenter(first_tile_position);
    this->region->visited = true;

    for(auto& pair : this->interface) {
        auto* component = pair.second.get();
        this->setVisibilityFalse(component->getWidgetID());
    }
}

void Regionmap::gamestateClose() {
    this->recalculate_tile_mesh = false;
    this->recalculate_tree_mesh = false;
    regionmap_mesh_tile.create(0);

    for(auto& pair : this->interface)
        this->setVisibilityFalse(pair.first);
}

void Regionmap::recalculateMesh() {
    this->recalculate_tile_mesh = true;
    this->recalculate_tree_mesh = true;
}

// void Regionmap::renderSelectedBuilding() {
    /*
    auto* building_menu = static_cast<gui::WidgetMenuBuilding*>(this->getInterfaceComponent("component_widget_menu_building"));
    auto  building      = building_menu->getBuilding();
    if(building != BUILDING_EMPTY && building_menu->isVisible()) {
        auto tile = this->region->getTileAtIndex(this->current_index);

        auto building_size = building.getBuildingArea(); 
        auto grid_position = tileGridPosition(this->current_index);
        
        for(int y = grid_position.y; y < grid_position.y + building_size.y; y++) {
            for(int x = grid_position.x; x < grid_position.x + building_size.x; x++) {
                auto grid = core::Vector2i(x, y);
                if(!inRegionBounds(grid)) 
                    return;
            }
        }
        
        building.object_position = tile.getPosition();
        
        const int a1_w = 0; 
        const int a1_h = tileSize().y; 
        const int r_w  = tileSize().x / 2;
        const int r_h  = tileSize().y;    
        
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
        auto tile_size     = tileSize(); 
        int  highlight_size_x = building_area * tile_size.x;
        int  highlight_size_y = building_area * tile_size.y;

        auto top    = building.getPosition2D().asSFMLVector2f() + sf::Vector2f(-(building_area - 1) * tile_size.x / 2, 0) + sf::Vector2f(highlight_size_x / 2, 0);
        auto right  = building.getPosition2D().asSFMLVector2f() + sf::Vector2f(-(building_area - 1) * tile_size.x / 2, 0) + sf::Vector2f(highlight_size_x, highlight_size_y / 2);
        auto left   = building.getPosition2D().asSFMLVector2f() + sf::Vector2f(-(building_area - 1) * tile_size.x / 2, 0) + sf::Vector2f(0, highlight_size_y / 2);
        auto bottom = building.getPosition2D().asSFMLVector2f() + sf::Vector2f(-(building_area - 1) * tile_size.x / 2, 0) + sf::Vector2f(highlight_size_x / 2, highlight_size_y);  

        sf::VertexArray building_surround_highlight(sf::Lines, 8);

        building_surround_highlight[0].position = top; 
        building_surround_highlight[1].position = right;
        building_surround_highlight[2].position = right;
        building_surround_highlight[3].position = bottom;
        building_surround_highlight[4].position = bottom;
        building_surround_highlight[5].position = left;
        building_surround_highlight[6].position = left;
        building_surround_highlight[7].position = top;

        building_surround_highlight[0].color = COLOUR_BLUE_RIVER.asSFMLColour();
        building_surround_highlight[1].color = COLOUR_BLUE_RIVER.asSFMLColour();
        building_surround_highlight[2].color = COLOUR_BLUE_RIVER.asSFMLColour();
        building_surround_highlight[3].color = COLOUR_BLUE_RIVER.asSFMLColour();
        building_surround_highlight[4].color = COLOUR_BLUE_RIVER.asSFMLColour();
        building_surround_highlight[5].color = COLOUR_BLUE_RIVER.asSFMLColour();
        building_surround_highlight[6].color = COLOUR_BLUE_RIVER.asSFMLColour();
        building_surround_highlight[7].color = COLOUR_BLUE_RIVER.asSFMLColour();

        game_manager.window.draw(building_surround_highlight);

        // Highlight.

        sf::VertexArray building_highlight(sf::Quads, 4);

        building_highlight[0].position = building.getPosition2D().asSFMLVector2f() + offset;
        building_highlight[1].position = building.getPosition2D().asSFMLVector2f() + offset + sf::Vector2f(building.getSize().x, 0);
        building_highlight[2].position = building.getPosition2D().asSFMLVector2f() + offset + sf::Vector2f(building.getSize().x, building.getSize().y); 
        building_highlight[3].position = building.getPosition2D().asSFMLVector2f() + offset + sf::Vector2f(0, building.getSize().y);

        building_highlight[0].texCoords = sf::Vector2f(0, 0);
        building_highlight[1].texCoords = sf::Vector2f(building.getSize().x, 0); 
        building_highlight[2].texCoords = sf::Vector2f(building.getSize().x, building.getSize().y); 
        building_highlight[3].texCoords = sf::Vector2f(0, building.getSize().y);

        auto invalid_position = !this->region->isBuildingPositionValid(building, grid_position); 
        if(invalid_position) {
            building_highlight[0].color = COLOUR_RED.asSFMLColour();
            building_highlight[1].color = COLOUR_RED.asSFMLColour();
            building_highlight[2].color = COLOUR_RED.asSFMLColour();
            building_highlight[3].color = COLOUR_RED.asSFMLColour();
        }

        const auto texture = building.getTextureName();
        
        sf::RenderStates states;
        states.texture = &game_manager.resource.getTexture(texture);
        game_manager.window.draw(building_highlight, states);

        const int area_x = building.getBuildingScanArea().x;
        const int area_y = building.getBuildingScanArea().y;
        
        if(area_x > 1 && area_y > 1) {
            int vertex_index = 0; 
            const size_t vertex_count = 4 * (2 * building.getBuildingScanArea().x + building_area) * (2 * building.getBuildingScanArea().y + building_area);
            sf::Vertex vertex[vertex_count];
            for(int y = -area_y; y <= area_y + building_area - 1; y++) {
                for(int x = -area_x; x <= area_x + building_area - 1; x++) {
                    const int index = this->current_index + calculateRegionIndex(x, y);
                    if(!inRegionBounds(index))
                        continue;

                    auto& tile = this->region->map[index];
                    auto tile_position = tile.getPosition2D().asSFMLVector2f();
                    auto tile_size     = tile.getSize().asSFMLVector2f();

                    sf::Vertex* quad = &vertex[vertex_index * 4];

                    quad[0].position = tile_position + sf::Vector2f(0, 0); 
                    quad[1].position = tile_position + sf::Vector2f(tile_size.x, 0); 
                    quad[2].position = tile_position + sf::Vector2f(tile_size.x, tile_size.y); 
                    quad[3].position = tile_position + sf::Vector2f(0, tile_size.y); 

                    quad[0].texCoords = sf::Vector2f(0, 0);
                    quad[1].texCoords = sf::Vector2f(tile_size.x, 0);
                    quad[2].texCoords = sf::Vector2f(tile_size.x, tile_size.y);
                    quad[3].texCoords = sf::Vector2f(0, tile_size.y);

                    auto colour = COLOUR_WHITE_TRANSPARENT90PERCENT.asSFMLColour();
                    if(building.isTileHarvestable(this->region, index))
                        colour = COLOUR_GREEN_TRANSPARENT90PERCENT.asSFMLColour();

                    else if(region->isSpotOccupied(index))
                        colour = COLOUR_RED_TRANSPARENT90PERCENT.asSFMLColour();

                    quad[0].color = colour;
                    quad[1].color = colour;
                    quad[2].color = colour;
                    quad[3].color = colour;

                    vertex_index++;
                }
            }

            sf::RenderStates states;
            states.texture = &game_manager.resource.getTexture("tile_transparent_white");
            game_manager.window.getWindow()->draw(vertex, vertex_count, sf::Quads, states);
        }
    }
    */
// }
}