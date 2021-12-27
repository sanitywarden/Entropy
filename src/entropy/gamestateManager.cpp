#include "gamestateManager.hpp"
#include "gamestate.hpp"

#include <iostream>

using namespace entropy;

gamestateManager::gamestateManager() {
    this->m_current_gamestate = nullptr;
}

gamestateManager::gamestateManager(const Settings& settings) {
    this->m_current_gamestate = nullptr;
    
    this->m_settings = settings;
}

gamestateManager::~gamestateManager() {
    
}

void gamestateManager::addGamestate(std::string id, entropy::Gamestate& gamestate) {
    this->m_gamestates[id] = &gamestate;
}

void gamestateManager::removeGamestate(std::string id) {
    this->m_gamestates.erase(id);
}

bool gamestateManager::checkGamestateExists(std::string id) {
    return this->m_gamestates.count(id);
}

void gamestateManager::setGamestate(std::string id) {
    auto* current_gamestate = this->m_current_gamestate;
    auto* new_gamestate     = this->m_gamestates[id];

    if(current_gamestate != nullptr)
        current_gamestate->gamestateClose();

    if(new_gamestate != nullptr)
        new_gamestate->gamestateLoad();

    this->m_current_gamestate = new_gamestate;
}

/* Returns current gamestate. */
Gamestate* entropy::gamestateManager::getGamestate() {
    return this->m_current_gamestate;
}

/*  Get a gamestate by name.
 *  If the requested gamestate does not exist, returns nullptr. */
Gamestate* entropy::gamestateManager::getGamestateByName(std::string id) {
    return this->checkGamestateExists(id)
        ? this->m_gamestates[id] 
        : nullptr; 
}