#ifndef _ENTROPY_CONTROLS_HPP_
#define _ENTROPY_CONTROLS_HPP_

#include "entropy.hpp"

#include <SFML/Window.hpp>
#include <string>
#include <map>


namespace entropy {    
    class Controls {
        friend class Entropy;

        public:
            std::map <std::string, sf::Keyboard::Key> key_map;
            std::map <std::string, bool>              key_state;
            bool mouse_left;
            bool mouse_right;
            bool mouse_middle;
            bool mouse_middle_up;   // Up 1, Down 0.
            bool mouse_middle_down; // Down 1, Up 0.
        public:
            Controls();
            ~Controls();

            void addKeyMapping(std::string id, sf::Keyboard::Key key);
            sf::Keyboard::Key getKeyMapping(std::string id);
            bool isKeyPressed(std::string id);        

            bool mouseLeftPressed();
            bool mouseRightPressed();
            bool mouseMiddlePressed();
            bool mouseMiddleUp();
            bool mouseMiddleDown();
            bool keyState(std::string id);

            bool addKeyMappingCheck(std::string id, sf::Keyboard::Key key);
            void removeKeyMapping(std::string id);
            bool checkKeyMappingExists(std::string id);
    };
}

#endif