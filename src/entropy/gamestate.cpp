#include "gamestate.hpp"

#include <iostream>

using namespace entropy;

Gamestate::Gamestate() {
    this->engine   = nullptr;
    this->state_id = "default";
}

Gamestate::Gamestate(Entropy* engine, std::string state_id) {
    this->engine   = engine;
    this->state_id = state_id;
}

Gamestate::~Gamestate() {
    
}

void Gamestate::updateMousePosition() {
    this->engine->window.getWindow()->setView(this->view_game);
    this->mouse_position_desktop = sf::Mouse::getPosition(*this->engine->window.getWindow());
    this->mouse_position_window  = this->engine->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    
    this->engine->window.getWindow()->setView(this->view_interface);
    this->mouse_position_interface = this->engine->window.getWindow()->mapPixelToCoords(this->mouse_position_desktop);
    this->engine->window.getWindow()->setView(this->view_game);
}