#ifndef _GUI_WIDGET_QUEST_HPP_
#define _GUI_WIDGET_QUEST_HPP_

#include "interfacePage.hpp"
#include "quest.hpp"

namespace gui {
    class WidgetQuest : public InterfacePage {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        public:
            iso::Quest quest; // The quest this GUI is for.
            
        public:
            WidgetQuest(iso::SimulationManager* manager);
            ~WidgetQuest();

            void createUI()      override;
            void updateUI()      override;
            void functionality() override;
    };
}

#endif