#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <map>
#include <vector>

namespace iso {    
class Controls {
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

        void addKeyMapping(const std::string& id, sf::Keyboard::Key key);
        bool addKeyMappingCheck(const std::string& id, sf::Keyboard::Key key);
        bool isKeyPressed(const std::string& id);        
        void removeKeyMapping(const std::string& id);
        bool checkKeyMappingExists(const std::string& id);

        bool mouseLeftPressed();
        bool mouseRightPressed();
        bool mouseMiddlePressed();
        bool mouseMiddleUp();
        bool mouseMiddleDown();
};
}