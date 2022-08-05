#ifndef _GUI_WIDGET_MENU_BUILDING_HPP_
#define _GUI_WIDGET_MENU_BUILDING_HPP_

#include "gui/base/interfacePage.hpp"
#include "globalutilities.hpp"
#include "building.hpp"

namespace gui {
    class WidgetMenuBuilding : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void createUI() override;
            void findBuilding();   
            sf::Vector2f calculateItemPosition(int building_no, int building_total); 
        private:
            iso::Building last_selected_building;
        
        public:
            WidgetMenuBuilding();
            WidgetMenuBuilding(iso::SimulationManager* manager);
            ~WidgetMenuBuilding();

            void updateUI() override;
            void resetBuilding();
            const iso::Building& getBuilding() const;
    };
}

#endif