#pragma once

#include "types.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <map>
#include <vector>

namespace iso {    
static std::map <std::string, sf::Keyboard::Key> gamestate_keys = {
    { "A", sf::Keyboard::Key::A },
    { "B", sf::Keyboard::Key::B },
    { "C", sf::Keyboard::Key::C },
    { "D", sf::Keyboard::Key::D },
    { "E", sf::Keyboard::Key::E },
    { "F", sf::Keyboard::Key::F },
    { "G", sf::Keyboard::Key::G },
    { "H", sf::Keyboard::Key::H },
    { "I", sf::Keyboard::Key::I },
    { "J", sf::Keyboard::Key::J },
    { "K", sf::Keyboard::Key::K },
    { "L", sf::Keyboard::Key::L },
    { "M", sf::Keyboard::Key::M },
    { "N", sf::Keyboard::Key::N },
    { "O", sf::Keyboard::Key::O },
    { "P", sf::Keyboard::Key::P },
    { "R", sf::Keyboard::Key::R },
    { "S", sf::Keyboard::Key::S },
    { "T", sf::Keyboard::Key::T },
    { "U", sf::Keyboard::Key::U },
    { "W", sf::Keyboard::Key::W },
    { "V", sf::Keyboard::Key::V },
    { "Q", sf::Keyboard::Key::Q },
    { "X", sf::Keyboard::Key::X },
    { "Y", sf::Keyboard::Key::Y },
    { "Z", sf::Keyboard::Key::Z },

    { "0", sf::Keyboard::Key::Num0 },
    { "1", sf::Keyboard::Key::Num1 },
    { "2", sf::Keyboard::Key::Num2 },
    { "3", sf::Keyboard::Key::Num3 },
    { "4", sf::Keyboard::Key::Num4 },
    { "5", sf::Keyboard::Key::Num5 },
    { "6", sf::Keyboard::Key::Num6 },
    { "7", sf::Keyboard::Key::Num7 },
    { "8", sf::Keyboard::Key::Num8 },
    { "9", sf::Keyboard::Key::Num9 },

    { "F1", sf::Keyboard::Key::F1 },
    { "F2", sf::Keyboard::Key::F2 },
    { "F3", sf::Keyboard::Key::F3 },
    { "F4", sf::Keyboard::Key::F4 },
    { "F5", sf::Keyboard::Key::F5 },
    { "F6", sf::Keyboard::Key::F6 },
    { "F7", sf::Keyboard::Key::F7 },
    { "F8", sf::Keyboard::Key::F8 },
    { "F9", sf::Keyboard::Key::F9 },
    { "F10", sf::Keyboard::Key::F10 },
    { "F11", sf::Keyboard::Key::F11 },
    { "F12", sf::Keyboard::Key::F12 },

    { "ARROW_LEFT" , sf::Keyboard::Key::Left  },
    { "ARROW_RIGHT", sf::Keyboard::Key::Right },
    { "ARROW_UP"   , sf::Keyboard::Key::Up    },
    { "ARROW_DOWN" , sf::Keyboard::Key::Down  },

    { "SPACEBAR" , sf::Keyboard::Key::Space     },
    { "TAB"      , sf::Keyboard::Key::Tab       },
    { "ENTER"    , sf::Keyboard::Key::Enter     },
    { "BACKSPACE", sf::Keyboard::Key::Backspace },
    { "ESCAPE"   , sf::Keyboard::Key::Escape    },
    { "MENU"     , sf::Keyboard::Key::Menu      },

    // It does not matter that the mapped key is left.
    // It's overriden by the KeyPressed and KeyReleased event to check for both the keys.
    { "SHIFT" , sf::Keyboard::Key::LShift   },
    { "CTRL"  , sf::Keyboard::Key::LControl },
    { "ALT"   , sf::Keyboard::Key::LAlt     },
    { "SYSTEM", sf::Keyboard::Key::LSystem  },
};

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

        core::Vector2i button_position_pressed;
        core::Vector2i button_position_released;
        sf::Vector2f resized;
        
        // ID of the last key pressed.
        std::string last_key_name; 

    public:
        Controls();
        ~Controls();

        void addKeyMapping(const std::string& id, sf::Keyboard::Key key);
        bool addKeyMappingCheck(const std::string& id, sf::Keyboard::Key key);
        bool isKeyPressed(const std::string& id) const;        
        void removeKeyMapping(const std::string& id);
        bool checkKeyMappingExists(const std::string& id) const;

        bool mouseLeftPressed();
        bool mouseRightPressed();
        bool mouseMiddlePressed();
        bool mouseMiddleUp();
        bool mouseMiddleDown();
};
}