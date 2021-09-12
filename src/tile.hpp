#ifndef _TILE_HPP_
#define _TILE_HPP_

#include <SFML/Graphics.hpp>

namespace iso {
    class Tile : public sf::Drawable {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        public:
            Tile();
            ~Tile();

            sf::Vector2i tile_position;
            sf::Vector2i tile_size;
            sf::Texture  tile_texture;
    };
}

#endif