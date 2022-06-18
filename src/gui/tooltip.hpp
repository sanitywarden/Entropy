#ifndef _GUI_TOOLTIP_HPP_
#define _GUI_TOOLTIP_HPP_

#include "interfacePage.hpp"

namespace gui {
    class Tooltip : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        public:  
            Tooltip();
            Tooltip(iso::SimulationManager* manager);
            ~Tooltip();

            void createUI()      override;
            void updateUI()      override;
            void functionality() override;
            bool intersectsSupportedUI() const;
            
    };
}

#endif