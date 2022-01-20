#ifndef _GUI_PERFORMANCE_HPP_
#define _GUI_PERFORMANCE_HPP_

#include "base/interfacePage.hpp"

namespace gui {
    class DebugPerformance : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void createUI() override;
        public:
            DebugPerformance();
            DebugPerformance(iso::SimulationManager* manager);
            ~DebugPerformance();

            void updateUI() override;
    };
}

#endif