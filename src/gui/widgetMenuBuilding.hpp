#ifndef _GUI_WIDGET_MENU_BUILDING_HPP_
#define _GUI_WIDGET_MENU_BUILDING_HPP_

#include "gui/base/interfacePage.hpp"
#include "./building/building_definitions.hpp"
#include "globalutilities.hpp"

namespace gui {
    class WidgetMenuBuilding : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void createUI() override;
            void findBuilding();   
            sf::Vector2f calculateItemPosition(int building_no, int building_total); 
        private:
            std::string last_selected_building;
        
        public:
            WidgetMenuBuilding();
            WidgetMenuBuilding(iso::SimulationManager* manager);
            ~WidgetMenuBuilding();

            void updateUI() override;
            void resetBuilding();
            std::shared_ptr <iso::Building> getBuilding();
    };
}

#endif