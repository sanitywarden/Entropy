#include "controls.hpp"

using namespace entropy;

Controls::Controls() {

}

Controls::~Controls() {

}

void Controls::addKeyMapping(std::string id, sf::Keyboard::Key key) {
    this->key_map.insert({id, key});
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

sf::Keyboard::Key Controls::getKeyMapping(std::string id) {
    return this->key_map[id];
}

bool Controls::checkKeyMappingExists(std::string id) {
    if(this->key_map.count(id))
        return true;
    return false;
}

bool Controls::isKeyPressed(std::string id) {
    if(sf::Keyboard::isKeyPressed(this->getKeyMapping(id)))
        return true;
    return false;
}