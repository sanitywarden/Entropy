#ifndef _GUI_INTERFACE_PAGE_HPP_
#define _GUI_INTERFACE_PAGE_HPP_

#include "abstractWidget.hpp"
#include "simulationManager.hpp"

#include "gui/base/abstractWidget.hpp"
#include "gui/base/button.hpp"
#include "gui/base/label.hpp"
#include "gui/base/image.hpp"
#include "gui/base/widget.hpp"

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

namespace gui {
    typedef std::shared_ptr <AbstractWidget> AbstractComponent; 
    typedef std::shared_ptr <Button>         ButtonComponent;
    typedef std::shared_ptr <Label>          LabelComponent;
    typedef std::shared_ptr <ImageHolder>    ImageComponent;
    typedef std::shared_ptr <Widget>         WidgetComponent;

    /*  This is a base class for GUI pages.
     *  GUI page is a high level GUI component ready to draw. It represents some interface in game.
     *  To create a new page you need to inherit from this class, override the virtual functions, and you are ready to go. */
    class InterfacePage : public AbstractWidget {
        protected:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { return; }
            virtual void createUI() { return; }
        protected:
            std::map <std::string, std::shared_ptr<AbstractWidget>> interface;
            iso::SimulationManager* manager;
        public:
            InterfacePage();
            InterfacePage(iso::SimulationManager* manager);
            ~InterfacePage();

            virtual void    updateUI()      { return; }
            virtual void    functionality() { return; }
            AbstractWidget* getComponent(std::string id) const;
            void            addComponent(std::shared_ptr<AbstractWidget> component);
            bool            intersectsUI(sf::Vector2f point) const;
    };
}

#endif