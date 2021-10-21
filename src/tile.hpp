#ifndef _TILE_HPP_
#define _TILE_HPP_

#include "entity.hpp"
#include "tileType.hpp"

#include <SFML/Graphics.hpp>
#include <bitset>

namespace iso {    
    class Tile : public sf::Drawable {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        public:
            Tile();
            ~Tile();

            Entity tree;
            std::vector <Entity> side;

            sf::Vector2f position;
            sf::Vector2f size;
            sf::Texture  texture;
            TileType     tiletype;

            int height;

    };
}

#endif