#ifndef _REGIONMAP_HPP_
#define _REGIONMAP_HPP_

#include "entropy/gamestate.hpp"
#include "entropy/entropy.hpp"
#include "region.hpp"
#include "worldGenerator.hpp"

#include <SFML/Graphics.hpp>

namespace iso {
    // Gamestate class responsible for management of a single region.
    class Regionmap : public entropy::Gamestate {
        private:
            worldGenerator* world;
            Region* m_region;

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

            void renderRegion();

            void selectTile();
            void unselectTile();
            void updateSelectedTile();
            void drawSelectedTile();
            void higlightTile();

            std::string getTilePixelColour(sf::Vector2i);

        public: 
            /* Regionmap's constructor is triggered every time the user visits a region. 
               This is because you somehow need to update the information about the state of the world, and the region the user visits. */
            Regionmap();
            Regionmap(entropy::Entropy* engine, iso::worldGenerator* world);
            ~Regionmap();

            void update() override;
            void render() override;

            void setCurrentRegion(int region_index);
    }; 
}

#endif