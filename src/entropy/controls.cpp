#include "controls.hpp"

using namespace iso;

Controls::Controls() {
    this->mouse_left        = false;
    this->mouse_right       = false;
    this->mouse_middle      = false;
    this->mouse_middle_up   = false;  
    this->mouse_middle_down = false;
    this->mouse_moved       = false;
}

Controls::~Controls() 
{}

void Controls::addKeyMapping(std::string id, sf::Keyboard::Key key) {
    this->key_map.insert({ id, key });
}

bool Controls::addKeyMappingCheck(std::string id, sf::Keyboard::Key key) {
    if(this->checkKeyMappingExists(id))
        return true;

    this->addKeyMapping(id, key);
    return false;
}

void Controls::removeKeyMapping(std::string id) {
    this->key_map.erase(id);
}

bool Controls::checkKeyMappingExists(std::string id) {
    return this->key_map.count(id);
}

bool Controls::isKeyPressed(std::string id) {
    if(this->checkKeyMappingExists(id))
        return this->key_state[id];
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