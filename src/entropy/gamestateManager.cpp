#include "gamestateManager.hpp"
#include "gamestate.hpp"

entropy::gamestateManager::gamestateManager() {
    this->m_current_gamestate = nullptr;
}

entropy::gamestateManager::~gamestateManager() {
    
}

void entropy::gamestateManager::addGamestate(const std::string& id, entropy::Gamestate& gamestate) {
    this->m_gamestates[id] = &gamestate;
}

void entropy::gamestateManager::removeGamestate(const std::string& id) {
    this->m_gamestates.erase(id);
}

void entropy::gamestateManager::setGamestate(const std::string& id) {
    this->m_current_gamestate = this->m_gamestates[id];
}

// Return current gamestate.
entropy::Gamestate& entropy::gamestateManager::getGamestate() {
    return *this->m_current_gamestate;
}