#ifndef _WORLD_SETTINGS_HPP_
#define _WORLD_SETTINGS_HPP_

#include <SFML/System/Vector2.hpp>

namespace iso {
    struct noiseSettings {
        int   octaves;
        int   persistence;
        int   bias;
        float multiplier;
        
        sf::Vector2f size;
    };  
    
    struct WorldSettings {
        sf::Vector2f size;
        sf::Vector2f panel_size;
        sf::Vector2i margin;
        
        float minimum_terrain_height; // Minimum noise value required for a region to be considered terrain and not water.

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

    struct RegionSettings {
        sf::Vector2f size;        // Size of the region.
        sf::Vector2f tile_size;   // Size of the tile.
        sf::Vector2f tile_offset; // Coordinates of the origin of the isometric world.
    };
}

#endif