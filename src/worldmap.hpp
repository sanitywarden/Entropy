#ifndef _WORLDMAP_HPP_
#define _WORLDMAP_HPP_

#include "entropy/gamestate.hpp"
#include "entropy/entropy.hpp"
#include "panel.hpp"
#include "generationSettings.hpp"
#include "worldGenerator.hpp"
#include "regionmap.hpp"
#include "gui/abstractWidget.hpp"
#include "gui/button.hpp"
#include "gui/widget.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>

namespace iso {
    class Worldmap : public entropy::Gamestate {
        private:
            Panel          panel;
            WorldSettings  world_settings;
            RegionSettings region_settings;
            worldGenerator world;
            Regionmap      region_gamestate;

            int selected_panel_index;

            std::map <std::string, gui::AbstractWidget*> m_interface;

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

            sf::Text debug_text;

            bool draw_debug;

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

            void createInterface();
            void updateInterface();
            void renderInterface();

        public:
            Worldmap(entropy::Entropy* engine);
            ~Worldmap();
            
            void update() override;
            void render() override;
            
    };
}

#endif