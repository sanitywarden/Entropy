#ifndef _WIDGET_SETTLE_CITY_HPP_
#define _WIDGET_SETTLE_CITY_HPP_

#include "interfacePage.hpp"

namespace gui {
    class WidgetSettleCity : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void createUI() override;
        public:
            WidgetSettleCity();
            WidgetSettleCity(iso::SimulationManager* manager);            
            ~WidgetSettleCity();

            void updateUI()      override;
            void functionality() override;
            void updateInput(std::string text); 

            bool acceptsInput();

        public:
            int region_id; // Index of the region where a city will be settled.
    };
}

#endif