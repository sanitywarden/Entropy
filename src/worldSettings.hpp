#ifndef _WORLD_SETTINGS_HPP_
#define _WORLD_SETTINGS_HPP_

#include <SFML/System/Vector2.hpp>

namespace iso {
    struct WorldSettings {
        sf::Vector2i size;
        sf::Vector2i panel_size;
        sf::Vector2i margin;
        
        int pole_size;
        int river_quantity;
        
        int octaves;     
        int persistence;
        int bias;
    };
}

#endif