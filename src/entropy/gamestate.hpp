#ifndef _ENTROPY_GAMESTATE_HPP_
#define _ENTROPY_GAMESTATE_HPP_

#include "entropy.hpp"

namespace entropy {
    // Base class for gamestates.
    // Gamestates represent possible states that the application can be in.
    // To create a new gamestate, inherit from this class, and override update(), render() and handleInput().
    class Gamestate {
        public:
            entropy::Entropy* engine;

            std::string state_id;
            sf::Event event;

            sf::Vector2i mouse_position_desktop;
            sf::Vector2f mouse_position_window;

            sf::View view_game;
            sf::View view_interface;

            Gamestate();
            ~Gamestate();

            // Main game loop functions.
            // Every gamestate has to override them in order to work properly.

            virtual void update() { return; }
            virtual void render() { return; }
            virtual void handleInput()   { return; }

            // Additional functions that might be helpful to keep gamestates interface similar.

            virtual void initialise()    { return; }
            virtual void loadResources() { return; }
            virtual void updateMousePosition() { return; }
            virtual void moveCamera()   { return; }
            virtual void zoomCamera()   { return; }
            virtual void updateCamera() { return; }
    };
}

#endif
