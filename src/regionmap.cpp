#include "regionmap.hpp"

using namespace iso;

Regionmap::Regionmap() {

}

Regionmap::Regionmap(entropy::Entropy* engine, worldGenerator* world, int region_index) {
    this->engine   = engine;
    this->state_id = "Regionmap";

    this->m_region = world->region_map[region_index];

    this->initialise();
    this->loadResources();
}

Regionmap::~Regionmap() {

}

void Regionmap::initialise() {
    this->view_game.setCenter(this->engine->window.getWindowSize().x / 2, this->engine->window.getWindowSize().y / 2);
    this->view_game.setSize(this->engine->window.getWindowSize());

    this->view_game.setCenter(this->engine->window.getWindowSize().x / 2, this->engine->window.getWindowSize().y / 2);
    this->view_game.setSize(this->engine->window.getWindowSize());
}

void Regionmap::loadResources() {

}

void Regionmap::update() {
    this->updateMousePosition();
    this->handleInput();
    this->updateCamera();

    this->engine->window.getWindow()->setView(this->view_game);

    this->engine->window.getWindow()->setView(this->view_interface);
}

void Regionmap::render() {
    this->engine->window.getWindow()->clear(sf::Color(100, 100, 100));    

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

        }
    }
}

void Regionmap::updateMousePosition() {

}

void Regionmap::moveCamera() {

}

void Regionmap::zoomCamera() {

}

void Regionmap::updateCamera() {

}