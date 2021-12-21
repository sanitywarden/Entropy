#include "gamestate.hpp"

using namespace entropy;

Gamestate::Gamestate() {
    this->engine = nullptr;
    this->state_id = "default";
}

Gamestate::~Gamestate() {
    
}

std::string Gamestate::getStateID() {
    return this->state_id;
}

sf::Vector2i Gamestate::getMousePositionDesktop() {
    return this->mouse_position_desktop;
}

sf::Vector2f Gamestate::getMousePositionWindow() {
    return this->mouse_position_window;
}

sf::Vector2f Gamestate::getMousePositionInterface() { 
    return this->mouse_position_interface;
}
