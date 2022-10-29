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
        Region*  region;
        Schedule scheduler;

        int tile_index;
        int region_index;

        bool recalculate_tile_mesh;
        bool recalculate_tree_mesh;
    private:
        void handleInput()     override;
        void initialise()      override;
        void loadResources()   override;
        void moveCamera()      override;
        void zoomCamera()      override;
        void updateCamera()    override;
        void gamestateLoad()   override;
        void gamestateClose()  override;

        void renderRegion();
        void higlightTile();
        // void renderSelectedBuilding();
        
    public: 
        Regionmap();
        ~Regionmap();

        void update(float delta_time) override;
        void render(float delta_time) override;
        void recalculateMesh();

        int L_getTileIndex() const;
        int L_getRegionIndex() const;
        void L_setRegionIndex(int index);
        
};
}