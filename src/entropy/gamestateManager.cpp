#include "gamestateManager.hpp"
#include "gamestate.hpp"

using namespace iso;

GamestateManager::GamestateManager()
    : current_gamestate(nullptr)
{}
    
GamestateManager::~GamestateManager() 
{}

void GamestateManager::addGamestate(const std::string& id, std::shared_ptr <Gamestate> gamestate) {
    this->gamestates[id] = gamestate;
}

void GamestateManager::removeGamestate(const std::string& id) {
    this->gamestates.erase(id);
}

bool GamestateManager::checkGamestateExists(const std::string& id) {
    return this->gamestates.count(id);
}

void GamestateManager::setGamestate(const std::string& id) {
    if(this->current_gamestate != nullptr)
        current_gamestate.get()->gamestateClose();

    this->current_gamestate = this->checkGamestateExists(id)
        ? this->gamestates[id]
        : nullptr;

    if(this->current_gamestate != nullptr)
        this->current_gamestate.get()->gamestateLoad();
}

/* Returns current gamestate. */
Gamestate* GamestateManager::getGamestate() {
    return this->current_gamestate.get();
}

/*  Get a gamestate by name.
 *  If the requested gamestate does not exist, returns nullptr. */
Gamestate* GamestateManager::getGamestateByName(const std::string& id) {
    return this->checkGamestateExists(id)
        ? this->gamestates[id].get() 
        : nullptr; 
}