#ifndef _WORLDMAP_HPP_
#define _WORLDMAP_HPP_

#include "entropy/gamestate.hpp"
#include "entropy/entropy.hpp"
#include "worldGenerator.hpp"
#include "simulationManager.hpp"
#include "regionmap.hpp"

#include "gui/performance.hpp"
#include "gui/widgetRegion.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>

namespace iso {
    class Worldmap : public entropy::Gamestate {
        friend class SimulationManager;

        private:
            Region             region;
            SimulationManager* manager;

            std::map <std::string, gui::InterfacePage*> interface;

            bool mouse_pressed;
            bool mouse_moved;
            bool mouse_drag;

            bool mouse_wheel_up;
            bool move_camera;
            bool zoom_camera;

            sf::Vector2f position_pressed;
            sf::Vector2f position_released;

            int zoom;
            int max_zoom_in;
            int max_zoom_out;

            bool draw_control_panel;

            int current_index;
            int selected_index;
            int draw_calls;
        private:
            void handleInput()         override;
            void initialise()          override;
            void loadResources()       override;
            void updateMousePosition() override;
            void moveCamera()          override;
            void zoomCamera()          override;
            void updateCamera()        override;

            void selectPanel();
            void unselectPanel();
            void updateSelectedPanel();
            void drawSelectedPanel();
            void highlightPanel();

            void renderWorld();
            void drawDebugText();
            
            void createUI();
            void updateUI(); 
            void renderUI();
            bool intersectsUI();

        public:
            Worldmap(SimulationManager* manager);
            ~Worldmap();
            
            void update(float time_per_frame) override;
            void render(float time_per_frame) override;
            
            int getCurrentIndex();
            int getSelectedIndex();
    };
}

#endif