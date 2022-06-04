#ifndef _WIDGET_REGION_STORAGE_HPP_
#define _WIDGET_REGION_STORAGE_HPP_

#include "interfacePage.hpp"

namespace gui {
    class WidgetRegionStorage : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void createUI() override;
            sf::Vector2f calculateItemPosition(int resource_no, int resource_total);
        private:
            bool should_refresh;

        public:
            WidgetRegionStorage();
            WidgetRegionStorage(iso::SimulationManager* manager);
            ~WidgetRegionStorage();

            void updateUI() override;
    };
}

#endif