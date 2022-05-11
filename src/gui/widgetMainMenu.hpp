#ifndef _WIDGET_MAIN_MENU_HPP_
#define _WIDGET_MAIN_MENU_HPP_

#include "interfacePage.hpp"

namespace gui {
    class WidgetMainMenu : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void createUI() override;
        public:
            WidgetMainMenu();
            WidgetMainMenu(iso::SimulationManager* manager);
            ~WidgetMainMenu();

            void updateUI()      override;
            void functionality() override;

    };
}

#endif
