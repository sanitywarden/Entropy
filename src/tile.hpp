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
            std::shared_ptr <Resource> resource;

            void setElevation(int elevation);
            int getElevation() const;
            
            bool hasResource() const;
            const Resource* getResource() const;
    };  
}

#endif