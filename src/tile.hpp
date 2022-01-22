#ifndef _TILE_HPP_
#define _TILE_HPP_

#include "tileType.hpp"
#include "gameObject.hpp"

#include <SFML/Graphics.hpp>

namespace iso {    
    class Tile : public GameObject {
        friend class WorldGenerator;

        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        protected:
            bool _marked;

        public:
            Tile();
            ~Tile();

            TileType tiletype;
            int elevation;
    };
}

#endif