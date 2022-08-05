#pragma once 

#include "tileType.hpp"
#include "gameObject.hpp"

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
