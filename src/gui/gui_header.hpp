#ifndef _GUI_HEADER_HPP_
#define _GUI_HEADER_HPP_

#include <SFML/Graphics.hpp>

#include <iostream>

#include "gui_widget.hpp"

namespace gui {
    class Header : public Widget {
        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

            sf::Text m_label;

        public:
            Header();
            ~Header();
            
            void setTextComponent(const sf::Text&);
            const sf::Text& getTextComponent();
    };
}

#endif