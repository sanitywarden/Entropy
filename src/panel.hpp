#ifndef _PANEL_HPP_
#define _PANEL_HPP_

#include <SFML/Graphics.hpp>

namespace iso {
    class Panel : public sf::Drawable {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            
        public:
            Panel();
            ~Panel();

            sf::Vector2f panel_position;
            sf::Vector2i panel_size;    
            sf::Texture  panel_texture;
            sf::Color    panel_colour;

            sf::Texture  *panel_feature_texture;
            
            float noise_value;

            bool is_terrain;
            bool is_arctic;
            bool is_coast;

            bool marked;
    };  
}

#endif