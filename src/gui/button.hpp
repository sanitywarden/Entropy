#ifndef _GUI_BUTTON_HPP_
#define _GUI_BUTTON_HPP_

#include "abstractWidget.hpp"
#include "text.hpp"

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui {
    class Button : public AbstractWidget {
        private:    
            Label m_button_text;

        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

        public:
            Button();
            ~Button();

            void setTextComponent(Label text_component);
            Label& textComponent();

            void update(std::function<void()>)               override;
            void onMouseButtonPress(std::function<void()>)   override;
            void onMouseButtonRelease(std::function<void()>) override;
            bool containsPoint(sf::Vector2f)                 override;
    };
}

#endif