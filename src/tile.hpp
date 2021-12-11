#ifndef _TILE_HPP_
#define _TILE_HPP_

#include "tileType.hpp"
#include "gameObject.hpp"

#include <SFML/Graphics.hpp>

namespace iso {    
    class Tile : public sf::Drawable {
        friend class worldGenerator;

        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        protected:
            bool _marked;

        public:
            Tile();
            ~Tile();

            GameObject tree;
            std::vector <GameObject> side;

            sf::Vector2f position;
            sf::Vector2f size;
            sf::Texture  texture;
            TileType     tiletype;

            int elevation;
    };
}

#endif