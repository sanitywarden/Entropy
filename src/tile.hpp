#ifndef _TILE_HPP_
#define _TILE_HPP_

#include "entity.hpp"

#include <SFML/Graphics.hpp>

namespace iso {
    class Tile : public sf::Drawable {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        public:
            Tile();
            ~Tile();

            Entity tree;

            sf::Vector2f position;
            sf::Vector2f size;
            sf::Texture  texture;
            
    };
}

#endif