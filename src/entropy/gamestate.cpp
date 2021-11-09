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