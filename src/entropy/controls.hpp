#ifndef _ENTROPY_CONTROLS_HPP_
#define _ENTROPY_CONTROLS_HPP_

#include <map>
#include <string>

#include <SFML/Window.hpp>

namespace entropy {
    class Controls {
        private:
            std::map <std::string, sf::Keyboard::Key> key_map;

        public:
            Controls();
            ~Controls();

            void addKeyMapping(std::string id, sf::Keyboard::Key key);
            
            /*  Check if a key under provided id exists.
             *  If such id does exists, returns true;
             *  If such id does not exist, key will be mapped to such id, returns false. */
            bool addKeyMappingCheck(std::string id, sf::Keyboard::Key key);
            void removeKeyMapping(std::string id);
            sf::Keyboard::Key getKeyMapping(std::string id);
            bool checkKeyMappingExists(std::string id);
            bool isKeyPressed(std::string id);
    };
}

#endif