#ifndef _ENTROPY_GAMESTATE_MANAGER_HPP_
#define _ENTROPY_GAMESTATE_MANAGER_HPP_

#include <map>
#include <string>
#include <memory>

namespace entropy {
    class Gamestate;
    
    // Gamestate Manager is a class that is used for storing and using user-created gamestates.
    class gamestateManager {
        private:
            std::map <std::string, entropy::Gamestate*> m_gamestates;
            entropy::Gamestate* m_current_gamestate;

        public:
            gamestateManager();
            ~gamestateManager();

            void addGamestate(const std::string& id, entropy::Gamestate& gamestate);
            void removeGamestate(const std::string& id);
            bool checkGamestateExists(const std::string& id);
            void setGamestate(const std::string& id);
            Gamestate& getGamestate();
    };
}

#endif