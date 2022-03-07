#ifndef _GUI_WIDGET_REGION_HPP_
#define _GUI_WIDGET_REGION_HPP_

#include "gui/base/interfacePage.hpp"

namespace gui {
    class WidgetRegion : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void createUI() override;

        public:
            WidgetRegion();
            WidgetRegion(iso::SimulationManager* manager);
            ~WidgetRegion();

            void updateUI()      override;
            void functionality() override;
    };
}

#endif