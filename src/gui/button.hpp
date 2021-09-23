#ifndef _GUI_BUTTON_HPP_
#define _GUI_BUTTON_HPP_

#include "abstractWidget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui {
    enum class TextAlignment {
        ALIGNED_LEFT,
        ALIGNED_RIGHT,
        CENTRED,
        DEFAULT
    };

    class Button : public AbstractWidget {
        private:    
            sf::Text m_button_text;

        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

        public:
            Button();
            ~Button();

            void alignTextComponent(TextAlignment alignment = TextAlignment::DEFAULT);
            void setTextComponent(sf::Text);
            sf::Text getTextComponent();

            void update()                                    override;
            void onMouseButtonPress(std::function<void()>)   override;
            void onMouseButtonRelease(std::function<void()>) override;
            bool containsPoint(sf::Vector2f)                 override;
    };
}

#endif