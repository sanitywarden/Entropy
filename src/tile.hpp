#ifndef _TILE_HPP_
#define _TILE_HPP_

#include "tileType.hpp"
#include "gameObject.hpp"
#include "resource.hpp"
#include "resource_definitions.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

namespace iso {    
    class Tile : public GameObject {
        friend class WorldGenerator;

        protected:
            bool _marked;

        public:
            Tile();
            ~Tile();

            TileType tiletype;

            void setElevation(int elevation);
            int getElevation() const;    
    };  
}

#endif