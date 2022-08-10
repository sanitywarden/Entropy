#ifndef _ENTROPY_CONTROLS_HPP_
#define _ENTROPY_CONTROLS_HPP_

#include "entropy.hpp"

#include <SFML/Window.hpp>
#include <string>
#include <map>
#include <vector>

extern std::vector <std::string> event_queue;

namespace iso {    
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
            bool mouse_moved;
            bool mouse_dragged;

            sf::Vector2f button_position_pressed;
            sf::Vector2f button_position_released;

            sf::Vector2f resized;

        public:
            Controls();
            ~Controls();

            void addKeyMapping(std::string id, sf::Keyboard::Key key);
            bool addKeyMappingCheck(std::string id, sf::Keyboard::Key key);
            bool isKeyPressed(std::string id);        
            void removeKeyMapping(std::string id);
            bool checkKeyMappingExists(std::string id);

            bool mouseLeftPressed();
            bool mouseRightPressed();
            bool mouseMiddlePressed();
            bool mouseMiddleUp();
            bool mouseMiddleDown();
    };
}

#endif