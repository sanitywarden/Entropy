#include "gamestateManager.hpp"
#include "gamestate.hpp"

entropy::gamestateManager::gamestateManager() {
    this->m_current_gamestate = nullptr;
}

entropy::gamestateManager::~gamestateManager() {
    
}

void entropy::gamestateManager::addGamestate(std::string id, entropy::Gamestate& gamestate) {
    this->m_gamestates[id] = &gamestate;
}

void entropy::gamestateManager::removeGamestate(std::string id) {
    this->m_gamestates.erase(id);
}

bool entropy::gamestateManager::checkGamestateExists(std::string id) {
    if(this->m_gamestates.count(id) > 0) 
        return true;
    return false;
}

void entropy::gamestateManager::setGamestate(std::string id) {
    this->m_current_gamestate = this->m_gamestates[id];
}

// Return current gamestate.
entropy::Gamestate* entropy::gamestateManager::getGamestate() {
    return this->m_current_gamestate;
}

entropy::Gamestate* entropy::gamestateManager::getGamestateByName(std::string id) {
    return this->m_gamestates.count(id) 
        ? this->m_gamestates[id] 
        : nullptr; 
}