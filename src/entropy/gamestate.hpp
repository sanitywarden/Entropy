#ifndef _ENTROPY_GAMESTATE_HPP_
#define _ENTROPY_GAMESTATE_HPP_

#include "entropy.hpp"
#include "controls.hpp"
#include "gui/base/interfacePage.hpp"

#include <map>

namespace entropy {
    // Base class for gamestates.
    // Gamestates represent possible states that the application can be in.
    // To create a new gamestate, inherit from this class, and override at least update(), render() and handleInput().
    class Gamestate {
        friend class Entropy;

        public:
            Entropy* engine;
            Controls controls;
            std::map <std::string, gui::InterfacePage*> interface;

            std::string  state_id;
            sf::Event    event;
            sf::Vector2i mouse_position_desktop;
            sf::Vector2f mouse_position_window;
            sf::Vector2f mouse_position_interface;
            sf::View     view_game;
            sf::View     view_interface;
            bool         move_camera;
            bool         zoom_camera;
            int          default_zoom;
            int          current_zoom;
            int          max_zoom_in;
            int          max_zoom_out;

        public:
            Gamestate();
            Gamestate(Entropy* engine, std::string state_id);
            ~Gamestate();

            /* Main game loop functions.
             * Every gamestate has to override these functions.
             * Delta time is passed as seconds, i.e: 0.001s => 1ms. */

            virtual void update(float delta_time) { return; }
            virtual void render(float delta_time) { return; }

            // Additional functions that might be helpful to keep gamestates interface clear.

            // This function will be called by the engine for a new set gamestate.
            virtual void gamestateLoad()  { return; }
            
            // This function will be called by the engine for the old gamestate, when a new one is set.
            virtual void gamestateClose() { return; }

            /* This function does not need to but may be overriden.
             * It provides default updates for 3 types of mouse position. 
             * Check entropy::Gamestate class for more details on mouse position types. */
            virtual void updateMousePosition();
            
            virtual void handleInput()          { return; }
            virtual void initialise()           { return; }
            virtual void loadResources()        { return; }
            virtual void moveCamera()           { return; }
            virtual void zoomCamera()           { return; }
            virtual void updateCamera()         { return; }
            virtual void updateScheduler()      { return; }
            virtual void createUI()             { return; } 

            int getDefaultZoom()    const;
            int getMaxZoomIn()      const;
            int getMaxZoomOut()     const;
            bool shouldCameraMove() const;
            bool shouldCameraZoom() const;

            void renderUI() const;
            void updateUI() const;
            void resizeUI() const;
            bool mouseIntersectsUI() const;
            bool pointIntersectsUI(sf::Vector2f point) const;

            void addInterfaceComponent(gui::InterfacePage* interface_component);
            gui::InterfacePage* getInterfaceComponent(std::string interface_id);
            bool checkComponentExist(std::string interface_id);
            void toggleComponentVisibility(std::string interface_id);
            void setVisibilityTrue(std::string interface_id);
            void setVisibilityFalse(std::string interface_id);
    };
}

#endif
