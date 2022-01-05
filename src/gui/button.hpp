#ifndef _GUI_BUTTON_HPP_
#define _GUI_BUTTON_HPP_

#include "abstractWidget.hpp"
#include "label.hpp"
#include "tileable.hpp"
#include "simulationManager.hpp"

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui {
    class Button : public AbstractWidget, public PropertyTileable {
        private:    
            iso::SimulationManager* manager;

        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            std::string getBlockTexture(int x, int y, sf::Vector2i dimensions) const override;
        public:
            Button();
            Button(iso::SimulationManager* manager, sf::Vector2i dimensions, std::string data);
            ~Button();

            Label label;
    };
}

#endif
