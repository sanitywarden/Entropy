#ifndef _WORLD_SETTINGS_HPP_
#define _WORLD_SETTINGS_HPP_

#include <SFML/System/Vector2.hpp>

namespace iso {
    struct NoiseSettings {
        NoiseSettings()
            : size(sf::Vector2f(0, 0)), octaves(0), persistence(0), bias(0), multiplier(0) {}

        NoiseSettings(sf::Vector2f size, int octaves, int persistence, int bias, float multiplier) 
            : size(size), octaves(octaves), persistence(persistence), bias(bias), multiplier(multiplier) {}

        int   octaves;
        int   persistence;
        int   bias;
        float multiplier;
        
        sf::Vector2f size;
    };  
    
    struct GenerationSettings {
        // World settings.
        // General settings.
        int world_size;                   // Side of the world area - x means world size is x * x.
        int world_margin_island;          // Distance between the edge of the world and the island.
        int world_margin_poles;           // Size of the poles.
        int world_river_quantity;         // Number of rivers that generation algorithm will attempt to generate.
        int world_river_scan_size;        // Side of the area scanned by river generation algorithm - x means area size is (2x + 1)(2x + 1). 
        float        world_noise_terrain; // From this value terrain will generate - has to be between 0 and 1.
        float        world_noise_forest;  // From this value forests will generate - has to be between 0 and 1.
        sf::Vector2f world_panel_size;
        

        // Noise settings.
        int world_noise_octaves;
        int world_noise_persistence;
        int world_noise_bias;
        float        world_noise_multiplier;

        // Moisture settings.
        int world_moisture_octaves;
        int world_moisture_persistence;
        int world_moisture_bias;
        float        world_moisture_multiplier;
    
        // Gradient settings.
        int world_gradient_octaves;
        int world_gradient_persistence;
        int world_gradient_bias;
        float        world_gradient_multiplier;
    
        // Region settings.
        // General.
        int region_size;                 // Side of the region area - x means region size is x * x.
        sf::Vector2f region_tile_size;   
        sf::Vector2f region_tile_offset; // By how many tiles is the region offset.
    };
}

#endif