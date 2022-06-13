#ifndef _MENU_HPP_
#define _MENU_HPP_

#include "entropy/gamestate.hpp"
#include "simulationManager.hpp"

#include "widgetMainMenu.hpp"
#include "performance.hpp"

namespace iso {
    class Menu : public Gamestate {
        friend class SimulationManager;

        public:
            SimulationManager* manager;

        private:
            void handleInput()    override;
            void initialise()     override;
            void loadResources()  override;
            void createUI()       override;
            void resizeViews()    override;

            void renderBackground();

        public:
            Menu(SimulationManager* manager);
            ~Menu();

            void update(float delta_time) override;
            void render(float delta_time) override;
    };
}

#endif