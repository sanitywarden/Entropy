#ifndef _GUI_BUTTON_HPP_
#define _GUI_BUTTON_HPP_

#include <SFML/Graphics.hpp>

#include <iostream>

#include "gui_widget.hpp"

namespace gui {
    class Button : public Widget {
        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

            sf::VertexArray m_button_shape;
            sf::Text m_button_text;

            bool m_is_transparent;

        public:
            Button();
            ~Button();

            void setTextComponent(const sf::Text&);
            const sf::Text& getTextComponent();

            void setTransparent(const bool&);
            const bool& isTransparent();

            bool containsPoint(const sf::Vector2f&)          override;
            void onMouseButtonPress(std::function<void()>)   override;
            void onMouseButtonRelease(std::function<void()>) override;
    };
}

#endif