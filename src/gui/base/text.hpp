#ifndef _GUI_TEXT_HPP_
#define _GUI_TEXT_HPP_

#include "abstractWidget.hpp"

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
        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

        private:
            sf::Text text;
        
        public:
            Label();
            Label(AbstractWidget* parent, sf::Text component);
            Label(AbstractWidget* parent, std::string text, sf::Font& font, unsigned int font_size);
            ~Label();

            // Align the text within it's parent.
            void align(Alignment alignment, sf::Vector2f offset = sf::Vector2f(0, 0));
            sf::Vector2f alignment() const;
            sf::Text& label();
    };
}

#endif