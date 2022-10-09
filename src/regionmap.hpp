#pragma once

#include "gamestate.hpp"
#include "region.hpp"
#include "schedule.hpp"

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace iso {
class Regionmap : public Gamestate {
    friend class SimulationManager;
    
    public:
        Region*            region;
        Schedule           scheduler;

        bool mouse_moved;
        bool mouse_drag;
        bool recalculate_mesh;
        bool recalculate_tree_mesh;

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
        Regionmap();
        ~Regionmap();

        void update(float delta_time) override;
        void render(float delta_time) override;

        Region* getCurrentRegion();
        int getRegionIndex() const;
        int getCurrentIndex() const;

        void recalculateMesh();
};
}