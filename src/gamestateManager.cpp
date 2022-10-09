#include "gamestateManager.hpp"
#include "gamestate.hpp"
#include "globalutilities.hpp"

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

bool GamestateManager::checkGamestateExists(const std::string& id) const {
    return this->gamestates.count(id);
}

void GamestateManager::setGamestate(const std::string& id) {
    if(this->current_gamestate != nullptr)
        current_gamestate.get()->gamestateClose();

    this->current_gamestate = this->checkGamestateExists(id)
        ? this->gamestates.at(id)
        : nullptr;
    
    if(this->current_gamestate != nullptr)
        this->current_gamestate.get()->gamestateLoad();

    else {
        printError("GamestateManager::setGamestate()", "Current gamestate is a nullptr.\n");
        return;
    }
}

/* Returns current gamestate. */
Gamestate* const GamestateManager::getGamestate() const {
    if(this->current_gamestate == nullptr) {
        printError("GamestateManager::getGamestate()", "Current gamestate is a nullptr.\n");
        return nullptr;
    }

    return this->current_gamestate.get();
}

/*  Get a gamestate by name.
 *  If the requested gamestate does not exist, returns nullptr. */
Gamestate* const GamestateManager::getGamestateByName(const std::string& id) const {
    if(!this->checkGamestateExists(id)) {
        printError("GamestateManager::getGamestateByName()", "Gamestate with id '" + id + "' does not exist.");
        return nullptr;
    }

    return this->gamestates.at(id).get();
}