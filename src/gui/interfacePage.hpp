#ifndef _GUI_INTERFACE_PAGE_HPP_
#define _GUI_INTERFACE_PAGE_HPP_

#include "abstractWidget.hpp"
#include "simulationManager.hpp"

#include "gui/abstractWidget.hpp"
#include "gui/button.hpp"
#include "gui/label.hpp"
#include "gui/image.hpp"
#include "gui/widget.hpp"

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace gui {
    /*  This is a base class for GUI pages.
     *  GUI page is a high level GUI component ready to draw. It represents some interface in game.
     *  To create a new page you need to inherit from this class, override the virtual functions, and you are ready to go. */
    class InterfacePage : public AbstractWidget {
        protected:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { return; }
            virtual void createUI() { return; }
        protected:
            std::map <std::string, AbstractWidget*> interface;
            iso::SimulationManager*                 manager;
        public:
            InterfacePage();
            InterfacePage(iso::SimulationManager* manager);
            ~InterfacePage();

            virtual void    updateUI() { return; }
            AbstractWidget* getComponent(std::string id) const;
            bool            intersectsUI(sf::Vector2f point) const;

        public:
            bool show;
    };
}

#endif