#ifndef _GUI_BUTTON_HPP_
#define _GUI_BUTTON_HPP_

#include "abstractWidget.hpp"
#include "text.hpp"

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui {
    class Button : public AbstractWidget {
        private:    
            Label message;

        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

        public:
            Button();
            Button(std::string message, sf::Font& font, unsigned int font_size);
            ~Button();

            void setTextComponent(const Label& text_component);
            Label& textComponent();

            bool containsPoint(sf::Vector2f) override;
    };
}

#endif