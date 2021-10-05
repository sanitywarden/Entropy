#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "biome.hpp"
#include "tile.hpp"
#include "entity.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

namespace iso {
    class worldGenerator;
    
    class Region : public sf::Drawable {
        friend class worldGenerator;
        
        protected:
            bool _terrain;
            bool _coast;
            bool _marked;

        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        public:
            Region();
            ~Region();

            sf::Vector2i grid;
            sf::Vector2f position;
            sf::Vector2f size;    
            sf::Texture  texture;

            Entity forest;
            Entity river;

            Biome biome;
            float height;
            float moisture;
            float temperature;
            float latitude;

            bool visited;

            std::vector <float> noise;
            std::vector <Tile>  map;

    };  
}

#endif