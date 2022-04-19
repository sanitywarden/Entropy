#ifndef _GUI_WIDGET_UNIT_HPP_
#define _GUI_WIDGET_UNIT_HPP_

#include "interfacePage.hpp"
#include "unit/unit.hpp"

namespace gui {
    class WidgetUnit : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void createUI() override;

            void deleteCurrentUnit();
            bool canColonise(int index) const;
        private:
            // What unit type is this GUI for.
            iso::Unit* unit;
        
        public:
            WidgetUnit();
            WidgetUnit(iso::SimulationManager* manager);
            ~WidgetUnit();

            void updateUI()      override;
            void functionality() override;
    };
}

#endif