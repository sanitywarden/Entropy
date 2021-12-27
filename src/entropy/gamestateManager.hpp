#ifndef _ENTROPY_GAMESTATE_MANAGER_HPP_
#define _ENTROPY_GAMESTATE_MANAGER_HPP_

#include <map>
#include <string>
#include <memory>

#include "engineSettings.hpp"

namespace entropy {
    class Gamestate;
    
    // Gamestate Manager is a class that is used for storing and using user-created gamestates.
    class gamestateManager {
        private:
            Settings m_settings;

            std::map <std::string, entropy::Gamestate*> m_gamestates;
            entropy::Gamestate* m_current_gamestate;

        public:
            gamestateManager();
            gamestateManager(const Settings& settings);
            ~gamestateManager();

            void addGamestate(std::string id, entropy::Gamestate& gamestate);
            void removeGamestate(std::string id);
            bool checkGamestateExists(std::string id);
            void setGamestate(std::string id);
            Gamestate* getGamestate();
            Gamestate* getGamestateByName(std::string id);
    };
}

#endif