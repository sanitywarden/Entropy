#ifndef _WORLD_SETTINGS_HPP_
#define _WORLD_SETTINGS_HPP_

#include <SFML/System/Vector2.hpp>

namespace iso {
    struct WorldSettings {
        sf::Vector2i size;
        sf::Vector2i panel_size;
        sf::Vector2i margin;
        
        int noise_octaves;     
        int noise_persistence;
        int noise_bias;
        int moisture_octaves;
        int moisture_persistence;
        int moisture_bias;
    
        int river_quantity;
        int pole_size;

        float multiplier_noise;
        float multiplier_gradient;
        float multiplier_moisture;
    };
}

#endif