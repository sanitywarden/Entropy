#include "worldmap.hpp"

using namespace iso;

Worldmap::Worldmap(entropy::Entropy* engine) {
    this->engine   = engine;
    this->state_id = "Worldmap";

    std::srand(time(0));

    this->initialise();
    this->loadResources();   

    this->world = worldGenerator(this->settings, this->engine); 
}

Worldmap::~Worldmap() {

}

void Worldmap::initialise() {
    this->mouse_pressed  = false;
    this->mouse_moved    = false;
    this->mouse_drag     = false;
    this->mouse_wheel_up = false;
    this->move_camera    = false;
    this->zoom_camera    = false;

    this->zoom = 0;
    this->max_zoom_in  = 0; 
    this->max_zoom_out = 3;

    this->settings.size.x         = 40;
    this->settings.size.y         = 40;
    this->settings.margin.x       = this->settings.size.x / 10;
    this->settings.margin.y       = this->settings.size.y / 10;
    this->settings.panel_size.x   = 64;
    this->settings.panel_size.y   = 32;
    this->settings.octaves        = 4;
    this->settings.persistence    = 4;
    this->settings.bias           = 2;
    this->settings.multiplier_noise    = 1.0f;
    this->settings.multiplier_gradient = 2.0f;

    this->view_game.setCenter(sf::Vector2f(
        this->settings.panel_size.x * this->settings.size.x / 2,
        this->settings.panel_size.y * this->settings.size.y / 2
    ));

    this->view_game.setSize(sf::Vector2f(
        this->settings.panel_size.x * this->settings.size.x / 15,
        this->settings.panel_size.y * this->settings.size.y / 15
    ));

    auto window_size = this->engine->window.getWindowSize();

    this->view_interface.setSize(window_size);
    this->view_interface.setCenter(window_size.x, window_size.y);
}

void Worldmap::loadResources() {
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_highlight",        sf::IntRect(0, 288, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_cold",       sf::IntRect(0, 0, 64, 32   ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_warm",       sf::IntRect(64, 0, 64, 32  ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_grass_tropical",   sf::IntRect(128, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_ocean",            sf::IntRect(192, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_sea",              sf::IntRect(256, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_arctic",           sf::IntRect(320, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_tundra",           sf::IntRect(384, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_desert",           sf::IntRect(448, 0, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_tl",  sf::IntRect(0, 32, 64, 32  ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_tr",  sf::IntRect(64, 32, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_bl",  sf::IntRect(0, 64, 64, 32  ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_corner_br",  sf::IntRect(64, 64, 64, 32 ));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_horizontal", sf::IntRect(128, 32, 64, 32));
    this->engine->resource.loadTexture("./res/panels/panel_atlas.png", "panel_river_vertical",   sf::IntRect(128, 64, 64, 32));

    this->engine->resource.loadFont("./res/font/proggy.ttf", "proggy");
}

void Worldmap::updateMousePosition() {
    this->mouse_position_desktop = sf::Mouse::getPosition(*this->engine->window.getWindow());
    this->mouse_position_window  = this->engine->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
}

void Worldmap::moveCamera() {
    auto distance = sf::Vector2f(
        (this->position_pressed.x - this->position_released.x) / this->settings.panel_size.x / 2,       
        (this->position_pressed.y - this->position_released.y) / this->settings.panel_size.y / 2
    );

    // Multipliers for faster camera movement. 
    float x_multiplier = 2.5f;
    float y_multiplier = 2.5f;    

    // Check the horizontal and vertical bounds of the screen.
    // This makes sure that you can not move past the world map.
    if(this->view_game.getCenter().x + (distance.x * x_multiplier) < (this->settings.size.x * this->settings.panel_size.x) - this->view_game.getSize().x / 2.f && this->view_game.getCenter().x + (distance.x * x_multiplier) > 0.f + this->view_game.getSize().x / 2.f) {
        this->view_game.move(x_multiplier * distance.x, 0.f);
    }

    if(this->view_game.getCenter().y + (distance.y * y_multiplier) < (this->settings.size.y * this->settings.panel_size.y) - this->view_game.getSize().y / 2.f && this->view_game.getCenter().y + (distance.y * y_multiplier) > 0.f + this->view_game.getSize().y / 2.f) {
        this->view_game.move(0.f, y_multiplier * distance.y);
    }

    this->move_camera = false;
}

void Worldmap::zoomCamera() {
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

void Worldmap::updateCamera() {
    if(this->move_camera) {
        this->moveCamera();
        this->move_camera   = false;
    }

    if(this->zoom_camera) {
        this->zoomCamera();
        this->zoom_camera = false;
    }

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
    if(this->view_game.getCenter().x + this->view_game.getSize().x / 2 > this->settings.panel_size.x * this->settings.size.x) {
        float offset_x = -(this->view_game.getCenter().x + this->view_game.getSize().x / 2);

        this->view_game.move(offset_x, 0.f);
    } 
    
    // Check if the camera is positioned badly on the bottom of the screen.
    if(this->view_game.getCenter().y + this->view_game.getSize().y / 2 > this->settings.panel_size.y * this->settings.size.y) {
        float offset_y = -(this->view_game.getCenter().y + this->view_game.getSize().y / 2);

        this->view_game.move(0.f, offset_y);
    }
}

void Worldmap::drawInterface() {
    sf::Text text;
    
    std::string str = "FPS: " + std::to_string(this->engine->fps.get()) + "\n";
    
    sf::Vector2i grid_position = sf::Vector2i(
        this->mouse_position_window.x / this->settings.panel_size.x,
        this->mouse_position_window.y / this->settings.panel_size.y
    );

    int index = grid_position.y * this->settings.size.x + grid_position.x;

    str += "Index: "   + std::to_string(index) + "\n";  
    str += "Noise: "   + std::to_string(this->world.world_map[index].noise_value) + "\n";
    str += "Terrain: " + std::to_string(this->world.world_map[index].is_terrain) + "\n";
    str += "Coast: "   + std::to_string(this->world.world_map[index].is_coast) + "\n";
    str += "Button pressed: " + std::to_string(this->mouse_pressed) + "\n";
    str += "Mouse moved: " + std::to_string(this->mouse_moved) + "\n";
    str += "Mouse dragged: " + std::to_string(this->mouse_drag) + "\n";

    auto window_size = this->engine->window.getWindowSize();

    text.setPosition(window_size.x / 2, window_size.y / 2);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(14);
    text.setFont(this->engine->resource.getFont("proggy"));
    text.setString(str);

    this->engine->window.getWindow()->draw(text);
}

void Worldmap::update() {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();
}

void Worldmap::render() {
    this->engine->window.clear(sf::Color(50, 50, 50));
    this->engine->window.getWindow()->setView(this->view_game);

    this->renderWorld();
    this->highlightPanel();

    this->engine->window.getWindow()->setView(this->view_interface);

    this->drawInterface();

    this->engine->window.getWindow()->setView(this->view_game);
    this->engine->window.display();
}

void Worldmap::handleInput() {
    while(this->engine->window.getWindow()->pollEvent(this->event)) {    
        switch(this->event.type) {
            case sf::Event::Closed: {
                entropy::Entropy::quitApplication(0);
                break;
            }

            case sf::Event::MouseButtonPressed: {
                this->mouse_pressed = true;

                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    this->position_pressed = this->mouse_position_window;
                    this->selectPanel();
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

    // Avoid accidental drags.
    if(this->mouse_drag && std::abs(this->position_pressed.x - this->position_released.x) > 10 && std::abs(this->position_pressed.y - this->position_released.y) > 10) this->move_camera = true;
}

void Worldmap::renderWorld() {
    for(auto& panel : this->world.world_map) {
        this->engine->window.getWindow()->draw(panel);
    }
}

void Worldmap::selectPanel() {
    if(this->mouse_pressed && !this->mouse_moved && !this->mouse_drag) {
        std::cout << "Pressing.\n";
    }
}

void Worldmap::unselectPanel() {

}

void Worldmap::updateSelectedPanel() {

}

void Worldmap::highlightPanel() {
    auto grid_position = sf::Vector2i(
        (int)this->mouse_position_window.x / this->settings.panel_size.x,
        (int)this->mouse_position_window.y / this->settings.panel_size.y
    );

    auto position = sf::Vector2f(
        grid_position.x * this->settings.panel_size.x,
        grid_position.y * this->settings.panel_size.y
    );

    sf::VertexArray highlight(sf::Quads, 4);

    highlight[0].position = position;
    highlight[1].position = position + sf::Vector2f(this->settings.panel_size.x, 0);
    highlight[2].position = position + sf::Vector2f(this->settings.panel_size.x, this->settings.panel_size.y);
    highlight[3].position = position + sf::Vector2f(0, this->settings.panel_size.y);

    highlight[0].texCoords = sf::Vector2f(0, 0);
    highlight[1].texCoords = sf::Vector2f(this->settings.panel_size.x, 0);
    highlight[2].texCoords = sf::Vector2f(this->settings.panel_size.x, this->settings.panel_size.y);
    highlight[3].texCoords = sf::Vector2f(0, this->settings.panel_size.y);

    sf::RenderStates states;
    states.texture = &this->engine->resource.getTexture("panel_highlight");

    this->engine->window.getWindow()->draw(highlight, states);
}