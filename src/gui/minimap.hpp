#ifndef _GUI_WIDGET_REGION_MINIMAP_HPP_
#define _GUI_WIDGET_REGION_MINIMAP_HPP_

#include "gui/base/interfacePage.hpp"

namespace gui {
class WidgetMinimap : public InterfacePage {
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void createUI() override;

    private:
        sf::Image minimap_image;

    public:
        WidgetMinimap();
        WidgetMinimap(iso::SimulationManager* manager);
        ~WidgetMinimap();

        void updateUI() override;      

    public:
        bool should_redraw;
};
}

#endif