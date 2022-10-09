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
        Region             region;
        // std::vector <Unit> units;

        bool mouse_moved;
        bool mouse_drag;
        
        sf::Vector2f position_pressed;
        sf::Vector2f position_released;

        int selected_unit_id;
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
        void updateScheduler() override;
        void createUI()        override;

        void selectPanel();
        void unselectPanel();
        void highlightPanel();

        void renderWorld();

        // void selectUnit();
        // void unselectUnit();
        // void selectUnitGoal();

        // Unit's and region's hitbox may be on top of each other, making it difficult to distinguish between each other.
        // This function returns a pointer to the object which is being clicked on.
        GameObject* getSelectedObject();

    public:
        Worldmap();
        ~Worldmap();
        
        void update(float delta_time) override;
        void render(float delta_time) override;
        
        int getCurrentIndex();
        int getSelectedIndex();
        int getSelectedUnitID();

        void centreWorldmap();
        void centreOnPlayerCapital();
};
}