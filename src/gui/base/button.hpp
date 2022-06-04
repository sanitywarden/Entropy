#ifndef _GUI_BUTTON_HPP_
#define _GUI_BUTTON_HPP_

#include "abstractWidget.hpp"
#include "label.hpp"
#include "tileable.hpp"
#include "simulationManager.hpp"
#include "image.hpp"

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui {
    class Button : public AbstractWidget, public PropertyTileable {
        private:    
            iso::SimulationManager* manager;
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            std::string getBlockTexture(int x, int y, sf::Vector2i dimensions) const override;
            std::string texture_id;
        public:
            Button();
            Button(iso::SimulationManager* manager, sf::Vector2i dimensions, std::string data);
            ~Button();

            Label label;
            void setWidgetTexture(std::string texture_name_base);
    };
}

#endif
