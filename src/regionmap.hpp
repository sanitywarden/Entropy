#ifndef _REGIONMAP_HPP_
#define _REGIONMAP_HPP_

#include "entropy/gamestate.hpp"
#include "simulationManager.hpp"
#include "region.hpp"
#include "./building/building_definitions.hpp"

#include "gui/widgetMenuBuilding.hpp"
#include "gui/performance.hpp"
#include "gui/minimap.hpp"
#include "gui/widgetRegionStorage.hpp"

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace iso {
    // Gamestate class responsible for management of a single region.
    class Regionmap : public entropy::Gamestate {
        friend class SimulationManager;

        public:
            Region*            region;
            SimulationManager* manager;
            Scheduler          scheduler;

            bool mouse_moved;
            bool mouse_drag;
            bool recalculate_mesh;
            bool recalculate_tree_mesh;
            bool rmesh;

            sf::Vector2f position_pressed;
            sf::Vector2f position_released;

            int current_index;
            int region_index;
            
        private:
            void handleInput()     override;
            void initialise()      override;
            void loadResources()   override;
            void moveCamera()      override;
            void zoomCamera()      override;
            void updateCamera()    override;
            void gamestateLoad()   override;
            void gamestateClose()  override;
            void updateScheduler() override;
            void createUI()        override;

            void renderRegion();
            void higlightTile();
            void renderSelectedBuilding();

            void updateTile();
            void updatePaths(int index);
        public: 
            Regionmap(SimulationManager* manager);
            ~Regionmap();

            void update(float delta_time) override;
            void render(float delta_time) override;

            void setCurrentRegion(int region_index);

            Region* getCurrentRegion();
            int getRegionIndex();
            int getCurrentIndex();
            int getVertexIndex();

            void recalculateMesh();
    };
}

#endif