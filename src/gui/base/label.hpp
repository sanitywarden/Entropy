#ifndef _GUI_TEXT_HPP_
#define _GUI_TEXT_HPP_

#include "abstractWidget.hpp"
#include "simulationManager.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

namespace gui {
    enum class Alignment {
        ALIGNMENT_CENTRED,
        ALIGMNENT_CENTRED_LEFT,
        ALIGMNENT_CENTRED_RIGHT,
        ALIGMNENT_CENTRED_TOP,
        ALIGMNENT_CENTRED_BOTTOM
    };

    class Label : public AbstractWidget {
        friend class Button;
        friend class Widget; 
        
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        private:
            iso::SimulationManager* manager;
            sf::Vector2f final_position;
            sf::Vector2f final_origin;
            std::string  data;
        public:
            Label();
            Label(iso::SimulationManager* manager);
            Label(iso::SimulationManager* manager, std::string data);
            ~Label();

            // Align the text within it's parent.
            void align(Alignment alignment, sf::Vector2f parent_position, sf::Vector2f parent_size);
            void setString(std::string data);
    };
}

#endif