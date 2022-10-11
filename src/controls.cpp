#include "controls.hpp"
#include <iostream>

using namespace iso;

Controls::Controls() {
    this->mouse_left        = false;
    this->mouse_right       = false;
    this->mouse_middle      = false;
    this->mouse_middle_up   = false;  
    this->mouse_middle_down = false;
    this->mouse_moved       = false;
    this->block_keyboard_input_ui = false;
}

Controls::~Controls() 
{}

void Controls::addKeyMapping(const std::string& id, sf::Keyboard::Key key) {
    this->key_map[id] = key;
}

bool Controls::addKeyMappingCheck(const std::string& id, sf::Keyboard::Key key) {
    if(this->checkKeyMappingExists(id))
        return true;

    this->addKeyMapping(id, key);
    return false;
}

void Controls::removeKeyMapping(const std::string& id) {
    this->key_map.erase(id);
}

bool Controls::checkKeyMappingExists(const std::string& id) const {
    return this->key_map.count(id);
}

bool Controls::isKeyPressed(const std::string& id) const {
    if(this->key_state.count(id))
        return this->key_state.at(id);
    return false;
}

bool Controls::mouseLeftPressed() {
    return this->mouse_left;
}

bool Controls::mouseRightPressed() {
    return this->mouse_right;
}

bool Controls::mouseMiddlePressed() {
    return this->mouse_middle;
}

bool Controls::mouseMiddleUp() {
    return this->mouse_middle_up;
}

bool Controls::mouseMiddleDown() {
    return this->mouse_middle_down;
}

void Controls::blockKeyboardInput(bool block) {
    this->block_keyboard_input_ui = block;
}

bool Controls::isInputBlocked() const {
    return this->block_keyboard_input_ui;
}