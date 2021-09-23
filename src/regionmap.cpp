#include "regionmap.hpp"

using namespace iso;

Regionmap::Regionmap() {

}

Regionmap::Regionmap(entropy::Entropy* engine, worldGenerator* world) {
    this->engine   = engine;
    this->world    = world;
    this->state_id = "Regionmap";

    this->m_region = nullptr;

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

    this->selected_panel_index = -1;

    this->zoom = 1;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 3;
    
    this->view_game.setCenter(this->engine->window.getWindowSize().x / 2, this->engine->window.getWindowSize().y / 2);
    this->view_game.setSize(this->engine->window.getWindowSize());

    this->view_interface.setCenter(this->engine->window.getWindowSize().x / 2, this->engine->window.getWindowSize().y / 2);
    this->view_interface.setSize(this->engine->window.getWindowSize());

}

void Regionmap::loadResources() {
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_warm",         sf::IntRect(0, 0, 64, 32   ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_cold",         sf::IntRect(64, 0, 64, 32  ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_subtropical",  sf::IntRect(128, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_grass_tropical",     sf::IntRect(192, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_tundra",             sf::IntRect(256, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_arctic",             sf::IntRect(320, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_desert",             sf::IntRect(384, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_ocean",              sf::IntRect(448, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_sea",                sf::IntRect(512, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_highlight",          sf::IntRect(0, 288, 64, 32 ));
    this->engine->resource.loadTexture("./res/tiles/tile_atlas.png", "tile_template_direction", sf::IntRect(64, 288, 64, 32));
}

void Regionmap::update() {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();
}

void Regionmap::render() {
    this->engine->window.getWindow()->clear(sf::Color(100, 100, 100));    

    this->engine->window.getWindow()->setView(this->view_game);

    this->renderRegion();
    this->higlightTile();

    this->engine->window.getWindow()->setView(this->view_interface);

    this->engine->window.getWindow()->display();
}

void Regionmap::handleInput() {
    while(this->engine->window.getWindow()->pollEvent(this->event)) {    
        switch(this->event.type) {
            case sf::Event::Closed: {
                entropy::Entropy::quitApplication(0);
                break;
            }

            case sf::Event::KeyPressed: {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
                    this->engine->gamestate.setGamestate("worldmap");
                }
            }

            case sf::Event::MouseButtonPressed: {
                this->mouse_pressed = true;

                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    this->position_pressed = this->mouse_position_window;
                }

                break;
            }

            case sf::Event::MouseButtonReleased: {
                this->mouse_pressed = false;

                break;
            }

            case sf::Event::MouseMoved: {
                this->mouse_moved = true;

                if(!this->mouse_drag) break;

                this->position_released = this->mouse_position_window;

                // Avoid accidental drags.
                // This also helps to record button presses (without the intention of dragging the screen).
                if(this->mouse_drag && std::abs(this->position_pressed.x - this->position_released.x) > 10 && std::abs(this->position_pressed.y - this->position_released.y) > 10) this->move_camera = true;

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
    if(this->mouse_moved && this->mouse_pressed) this->mouse_drag = true;

    else {
        this->mouse_drag = false;
        this->mouse_moved = false;
    }
}

void Regionmap::updateMousePosition() {
    this->engine->window.getWindow()->setView(this->view_game);
    
    this->mouse_position_desktop = sf::Mouse::getPosition(*this->engine->window.getWindow());
    this->mouse_position_window  = this->engine->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    
    this->engine->window.getWindow()->setView(this->view_interface);

    this->mouse_position_interface = this->engine->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    
    this->engine->window.getWindow()->setView(this->view_game);
}

void Regionmap::moveCamera() {
    auto division = this->world->getRegionSettings().tile_size.x + this->world->getRegionSettings().tile_size.y;
    auto distance = sf::Vector2f(
        (this->position_pressed.x - this->position_released.x) / division,       
        (this->position_pressed.y - this->position_released.y) / division
    );

    // Multipliers for faster camera movement. 
    float x_multiplier = 6.0f;
    float y_multiplier = 6.0f;    

    // Check the horizontal and vertical bounds of the screen.
    // This makes sure that you can not move past the world map.
    // if(this->view_game.getCenter().x + (distance.x * x_multiplier) < (this->world_settings.size.x * this->world_settings.panel_size.x) - this->view_game.getSize().x / 2.f && this->view_game.getCenter().x + (distance.x * x_multiplier) > 0.f + this->view_game.getSize().x / 2.f) {
        this->view_game.move(x_multiplier * distance.x, 0.f);
    // }

    // if(this->view_game.getCenter().y + (distance.y * y_multiplier) < (this->world_settings.size.y * this->world_settings.panel_size.y) - this->view_game.getSize().y / 2.f && this->view_game.getCenter().y + (distance.y * y_multiplier) > 0.f + this->view_game.getSize().y / 2.f) {
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
    if(this->m_region != nullptr) {
        for(auto& tile : this->m_region->map) {
            this->engine->window.getWindow()->draw(tile);
        }
    }
}

void Regionmap::setCurrentRegion(int region_index) {
    this->m_region = &this->world->region_map[region_index];

    // Here you can setup what to do when entering a region.

    auto first_tile_position = sf::Vector2f(
        this->m_region->map[0].tile_position.x + this->world->getRegionSettings().tile_size.x / 2,
        this->m_region->map[0].tile_position.y
    );

    this->view_game.setCenter(first_tile_position);
}

void Regionmap::selectTile() {

}

void Regionmap::unselectTile() {

}

void Regionmap::updateSelectedTile() {
    
}

void Regionmap::drawSelectedTile() {

}

void Regionmap::higlightTile() {
    auto tile_size   = this->world->getRegionSettings().tile_size;
    auto tile_offset = this->world->getRegionSettings().tile_offset;

    auto tile_grid_position = sf::Vector2i(
        this->mouse_position_window.x / tile_size.x,
        this->mouse_position_window.y / tile_size.y
    );

    auto selected = sf::Vector2i(
        (tile_grid_position.y - tile_offset.y) + (tile_grid_position.x - tile_offset.x),
        (tile_grid_position.y - tile_offset.y) - (tile_grid_position.x - tile_offset.x)
    );

    auto pixel = sf::Vector2i(
        (int)this->mouse_position_window.x % tile_size.x,
        (int)this->mouse_position_window.y % tile_size.y
    );  

    auto pixel_colour = this->getTilePixelColour(pixel);

    if(pixel_colour == "Red")
        selected += sf::Vector2i(-1, 0);        
    else if(pixel_colour == "Green")
        selected += sf::Vector2i(1, 0);
    else if(pixel_colour == "Blue")
        selected += sf::Vector2i(0, -1);
    else if(pixel_colour == "Yellow")
        selected += sf::Vector2i(0, 1);

    auto position_transformed = sf::Vector2f(
        this->world->tilePositionScreen(selected.x, selected.y).x,
        this->world->tilePositionScreen(selected.x, selected.y).y
    );

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
    states.texture = &this->engine->resource.getTexture("tile_highlight");

    this->engine->window.getWindow()->draw(highlight, states);
}

std::string Regionmap::getTilePixelColour(sf::Vector2i pixel) {
    if(pixel.x < 0 || pixel.y < 0) 
        return "Other";

    if(pixel.x > this->world->getRegionSettings().tile_size.x - 1 || pixel.y > this->world->getRegionSettings().tile_size.y - 1) 
        return "Other";

    auto pixel_colour = this->engine->resource.getTexture("tile_template_direction").copyToImage().getPixel(pixel.x, pixel.y);

    if(pixel_colour == sf::Color::Red)         return "Red";
    else if(pixel_colour == sf::Color::Green)  return "Green";
    else if(pixel_colour == sf::Color::Blue)   return "Blue";
    else if(pixel_colour == sf::Color::Yellow) return "Yellow";
    else return "Other";
}