#pragma once

#include <map>
#include <string>
#include <memory>

namespace iso {
class Gamestate;
class GamestateManager {
    private:
        std::map <std::string, std::shared_ptr<Gamestate>> gamestates;
        std::shared_ptr<Gamestate> current_gamestate;

    public:
        GamestateManager();
        ~GamestateManager();

        void addGamestate(const std::string& id, std::shared_ptr <Gamestate> gamestate);
        void removeGamestate(const std::string& id);
        bool checkGamestateExists(const std::string& id);
        void setGamestate(const std::string& id);
        Gamestate* getGamestate();
        Gamestate* getGamestateByName(const std::string& id);
};
}