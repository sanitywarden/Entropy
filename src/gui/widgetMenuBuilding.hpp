#ifndef _GUI_WIDGET_MENU_BUILDING_HPP_
#define _GUI_WIDGET_MENU_BUILDING_HPP_

#include "gui/interfacePage.hpp"

namespace gui {
    class WidgetMenuBuilding : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void createUI() override;
        public:
            WidgetMenuBuilding();
            WidgetMenuBuilding(iso::SimulationManager* manager);
            ~WidgetMenuBuilding();

            // void updateUI() const override;
    };
}

#endif