#ifndef _WORLDMAP_HPP_
#define _WORLDMAP_HPP_

#include "entropy/gamestate.hpp"
#include "panel.hpp"
#include "worldSettings.hpp"
#include "worldGenerator.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>

namespace iso {
    class Worldmap : public entropy::Gamestate {
        private:
            Panel          panel;
            WorldSettings  settings;
            worldGenerator world;

            int selected_panel_index;

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
            void highlightPanel();

            void renderWorld();

            void drawInterface();

        public:
            Worldmap(entropy::Entropy* engine);
            ~Worldmap();
            
            void update() override;
            void render() override;
            
    };
}

#endif