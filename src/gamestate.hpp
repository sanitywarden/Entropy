#pragma once

#include "gui/interfacePage.hpp"
#include "controls.hpp"

#include <map>
#include <string>

namespace iso {
class Gamestate {
    public:
        std::map <std::string, gui::InterfaceComponent> interface;
        Controls controls;

        std::string  state_id;
        sf::Event    event;
        core::Vector2i mouse_position_desktop;
        core::Vector2i mouse_position_window;
        core::Vector2i mouse_position_interface;
        sf::View     view_game;
        sf::View     view_interface;
        bool         move_camera;
        bool         zoom_camera;
        int          default_zoom;
        int          current_zoom;
        int          max_zoom_in;
        int          max_zoom_out;

    public:
        Gamestate(const std::string& state_id);
        ~Gamestate();

        /* Main game loop functions.
         * Every gamestate has to override these functions.
         * Delta time is passed as seconds, i.e: 0.001s => 1ms. */

        virtual void update(float delta_time) { return; }
        virtual void render(float delta_time) { return; }

        // This function will be called by the engine for a new set gamestate.
        virtual void gamestateLoad()  { return; }
        
        // This function will be called by the engine for the old gamestate, when a new one is set.
        virtual void gamestateClose() { return; }

        /* This function does not need to but may be overriden.
         * It provides default updates for 3 types of mouse position. 
         * Check Gamestate class for more details on mouse position types. */
        virtual void updateMousePosition();
        
        virtual void handleInput()          { return; }
        virtual void initialise()           { return; }
        virtual void loadResources()        { return; }
        virtual void moveCamera()           { return; }
        virtual void zoomCamera()           { return; }
        virtual void updateCamera()         { return; }
        virtual void updateScheduler()      { return; }
        virtual void createUI()             { return; } 
        virtual void resizeViews();

        const std::string& getStateId() const;
        int getDefaultZoom()            const;
        int getMaxZoomIn()              const;
        int getMaxZoomOut()             const;
        bool shouldCameraMove()         const;
        bool shouldCameraZoom()         const;

        void renderUI() const;
        void redrawUI() const;
        void resizeUI();
        void updateUI() const;
        bool mouseIntersectsUI() const;
        bool pointIntersectsUI(core::Vector2i point) const;
        bool mouseIntersectsUI(const std::string& component_name) const;
        bool pointIntersectsUI(core::Vector2i point, const std::string& component_name) const;

        void addInterfaceComponent(gui::InterfaceComponent interface_component);
        gui::InterfacePage* const getInterfaceComponent(const std::string& interface_id) const;
        bool checkComponentExist(const std::string& interface_id) const;
        void toggleComponentVisibility(const std::string& interface_id);
        bool isComponentVisible(const std::string& interface_id) const;
        void setVisibilityTrue(const std::string& interface_id);
        void setVisibilityFalse(const std::string& interface_id);
        void runGUIEventHandle(const std::string& event_name);
};
}