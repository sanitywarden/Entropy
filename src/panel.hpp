#ifndef _PANEL_HPP_
#define _PANEL_HPP_

#include "panelFeature.hpp"

#include <SFML/Graphics.hpp>

namespace iso {
    // Panel features reside in this struct.
    // Features are special elements that are rare or unique to a panel, such as rivers or forests.
    struct Feature {
        panelFeature river;
    };
    
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

            Feature feature;

            float noise_value;
            int   temperature;
            float latitude;

            bool is_river;
            bool is_terrain;
            bool is_arctic;
            bool is_coast;

            bool marked;
    };  
}

#endif