#ifndef _GUI_TEXT_HPP_
#define _GUI_TEXT_HPP_

#include "abstractWidget.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

namespace gui {
    class Label : public AbstractWidget {
        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

        private:
            sf::Text     m_text_component;
            sf::Vector2f m_text_alignment;    
        
        public:
            Label();
            Label(AbstractWidget* parent, sf::Text component, sf::Vector2f alignment);
            Label(AbstractWidget* parent, std::string text, sf::Font& font, unsigned int font_size, sf::Vector2f alignment);
            ~Label();

            // Align the text within it's parent.
            void align(sf::Vector2f alignment);
            sf::Vector2f alignment() const;
            sf::Text& label();
    };
}

#endif