#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "biome.hpp"
#include "tile.hpp"
#include "regionType.hpp"
#include "gameObject.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

namespace iso {
    class worldGenerator;
    
    enum class RiverDirection {
        RIVER_HORIZONTAL,
        RIVER_VERTICAL,
        RIVER_NORTH_TO_EAST,
        RIVER_NORTH_TO_WEST,
        RIVER_SOUTH_TO_EAST,
        RIVER_SOUTH_TO_WEST, 
        RIVER_NONE,          // River does not exist.
        RIVER_ORIGIN         // Beginning of the river, first tile.
    };

    class Region : public sf::Drawable {
        friend class worldGenerator;
        
        protected:
            bool _marked;
            RiverDirection _direction;      

        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        public:
            Region();
            ~Region();

            sf::Vector2i grid;
            sf::Vector2f position;
            sf::Vector2f size;    
            sf::Texture  texture;
            RegionType   regiontype;

            GameObject forest;
            GameObject river;

            Biome biome;
            float height;
            float moisture;
            float temperature;
            float latitude;

            bool visited;

            std::vector <Tile>     map;

            RiverDirection riverDirection();
    };  
}

#endif