#ifndef _REGIONMAP_HPP_
#define _REGIONMAP_HPP_

#include "entropy/gamestate.hpp"
#include "simulationManager.hpp"
#include "region.hpp"

#include "pawn.hpp"

#include "gui/widgetMenuBuilding.hpp"
#include "gui/performance.hpp"

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <bitset>

namespace iso {
    // Gamestate class responsible for management of a single region.
    class Regionmap : public entropy::Gamestate {
        friend class SimulationManager;

        private:
            Region*            region;
            SimulationManager* manager;

            std::map <std::string, gui::InterfacePage*> interface;
            std::map <std::string, UpdateUtility>       scheduler;

            Pawn pawn;
            int frames;

            bool mouse_pressed;
            bool mouse_moved;
            bool mouse_drag;
            bool move_camera;
            bool zoom_camera;

            sf::Vector2f position_pressed;
            sf::Vector2f position_released;

            int zoom;
            int max_zoom_in;
            int max_zoom_out;

            int current_index;
            int region_index;
            int draw_calls;
        private:
            void handleInput()         override;
            void initialise()          override;
            void loadResources()       override;
            void moveCamera()          override;
            void zoomCamera()          override;
            void updateCamera()        override;

            void renderRegion();
            void higlightTile();

            void updateTile();
            void updatePaths(int index);

            void createUI();
            void renderUI();
            void updateUI();
            bool intersectsUI();
        public: 
            Regionmap(SimulationManager* manager);
            ~Regionmap();

            void update(float delta_time) override;
            void render(float delta_time) override;

            void setCurrentRegion(int region_index);

            Region* getCurrentRegion();
            int getRegionIndex();
            int getCurrentIndex();
            int getDrawCalls();
    };
}

#endif