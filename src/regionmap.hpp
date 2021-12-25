#ifndef _REGIONMAP_HPP_
#define _REGIONMAP_HPP_

#include "entropy/gamestate.hpp"
#include "simulationManager.hpp"
#include "region.hpp"
#include "buildingManager.hpp"

#include <SFML/Graphics.hpp>

namespace iso {
    // Gamestate class responsible for management of a single region.
    class Regionmap : public entropy::Gamestate {
        friend class SimulationManager;

        private:
            Region*            region;
            SimulationManager* manager;
            BuildingManager    building_manager;

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

            int current_index;
            int draw_calls;
        private:
            void handleInput()         override;
            void initialise()          override;
            void loadResources()       override;
            void updateMousePosition() override;
            void moveCamera()          override;
            void zoomCamera()          override;
            void updateCamera()        override;

            void renderRegion();
            void higlightTile();
            void drawDebugText();

            void updateTile();

        public: 
            Regionmap();
            Regionmap(SimulationManager* manager);
            ~Regionmap();

            void update(float time_per_frame) override;
            void render(float time_per_frame) override;

            void setCurrentRegion(int region_index);

            int getCurrentIndex();
            Region* getCurrentRegion();
    }; 
}

#endif