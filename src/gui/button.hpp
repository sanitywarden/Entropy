#ifndef _GUI_BUTTON_HPP_
#define _GUI_BUTTON_HPP_

#include "abstractWidget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui {
    enum class TextAlignment {
        ALIGNED_LEFT,
        ALIGNED_RIGHT,
        CENTRED
    };

    class Button : public AbstractWidget {
        private:    
            sf::VertexArray m_button_shape;
            bool            m_transparent;
            sf::Text        m_button_text;

        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

        public:
            sf::Color colour;

        public:
            Button();
            ~Button();

            void setTextComponent(sf::Text&);
            sf::Text& getTextComponent();

            // void alignTextComponent(TextAlignment);

            void setTransparent(bool&);
            bool& isTransparent();

            bool containsPoint(const sf::Vector2f&)          override;
            void onMouseButtonPress(std::function<void()>)   override;
            void onMouseButtonRelease(std::function<void()>) override;
    };
}

#endif