#pragma once

#include "gamestate.hpp"
#include "worldGenerator.hpp"
#include "regionmap.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cmath>

namespace iso {
class Worldmap : public Gamestate {
    friend class SimulationManager;

    public:
        int current_index;
        int selected_index;
        std::string lens;

    private:
        void handleInput()     override;
        void initialise()      override;
        void loadResources()   override;
        void moveCamera()      override;
        void zoomCamera()      override;
        void updateCamera()    override;
        void gamestateLoad()   override;
        void gamestateClose()  override;

        void highlightPanel();

        void renderWorld();
    public:
        Worldmap();
        ~Worldmap();
        
        void update(float delta_time) override;
        void render(float delta_time) override;
        
        int L_getCurrentIndex() const;
        int L_getSelectedIndex() const; 
        void L_setSelectedIndex(int index);

        void centreWorldmap();
        void centreOnPlayerCapital();
};
}