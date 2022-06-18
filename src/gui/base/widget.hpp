#ifndef _GUI_WIDGET_HPP_
#define _GUI_WIDGET_HPP_

#include "abstractWidget.hpp"
#include "simulationManager.hpp"
#include "button.hpp"
#include "label.hpp"
#include "tileable.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <map>

namespace gui {
    class Widget : public AbstractWidget, public PropertyTileable {
        private:
            iso::SimulationManager* manager;
            
        protected:
            std::map <std::string, AbstractWidget*> m_components;
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            std::string getBlockTexture(int x, int y, sf::Vector2i dimensions) const override;
            
        public:
            Widget(); 
            Widget(iso::SimulationManager* manager, int x, int y);
            Widget(iso::SimulationManager* manager, sf::Vector2i dimensions);
            ~Widget();

            void addComponent(AbstractWidget* component, std::string id);
            AbstractWidget* getComponentByName(std::string id);
    };
}

#endif
