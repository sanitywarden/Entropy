#ifndef _WORLD_SETTINGS_HPP_
#define _WORLD_SETTINGS_HPP_

#include "noiseSettings.hpp"

#include <SFML/System/Vector2.hpp>

namespace iso {    
// TODO:
// - Add more settings, that currently are hardcoded, such as the chance to generate stone in a region.
// - Make the settings load from a config file.
// - Settings: Biome generation settings.
// - Maybe make this class hold region data, that are currently stored by the SimulationManager class.

class WorldData {
    public:
        // World settings.
        // General settings.
        
        int world_size;            // Side of the world area - x means world size is x * x.
        int world_margin_poles;    
        int world_margin_island;   // Distance between the edge of the world and the island.
        int world_river_quantity;  // Number of rivers that generation algorithm will attempt to generate.
        int world_river_scan_size; // Side of the area scanned by river generation algorithm - x means area size is (2x + 1)(2x + 1). 
        float world_noise_terrain; // From this value terrain will generate - has to be between 0 and 1.
        float world_noise_forest;  // From this value forests will generate - has to be between 0 and 1.
        int world_panel_size;
        
        // Noise settings.

        // Region settings.
        // General settings.
        
        int region_size;                 // Side of the region area - x means region size is x * x.
        sf::Vector2f region_tile_size;   
        sf::Vector2f region_tile_offset; // By how many tiles is the region offset.

        // Region resources.

        float resource_flint_chance; // Chance to generate flint in a region (0.00 - 1.00).
        float resource_stone_chance; // Chance to generate stone in a region (0.00 - 1.00).

        int resource_flint_radius; // How big the flint resource patch should be.
        int resource_stone_radius; // How big the stone resource patch should be.

        float tree_noise_minimum; // Smallest noise value acceptable to generate a tree tile (0.00 - 1.00).
        float flint_noise_minimum; // Smallest noise value acceptable to generate a flint tile (0.00 - 1.00).
        float stone_noise_minimum; // Smallest noise value acceptable to generate a stone tile (0.00 - 1.00).

        // Game data.

        int player_quantity; // Number of players in the game.

    private:
        // This should only be called once in the constructor.
        // It is a function only for readability.
        void loadSettingsFromFile();
    
    public:
        WorldData();
        ~WorldData();

        // Worldmap API

        int getWorldSize() const;
        int calculateWorldIndex(int x, int y) const;
        bool inWorldBounds(int index) const;
        NoiseSettings getWorldNoiseSettings() const;
        NoiseSettings getWorldMoistureSettings() const;
        NoiseSettings getWorldGradientSettings() const;
        NoiseSettings getWorldForestSettings() const;

        // Regionmap API

        int getRegionSize() const;
        int calculateRegionIndex(int x, int y) const;
        bool inRegionBounds(int index) const;

        NoiseSettings getRegionForestSettingsDense() const;
        NoiseSettings getRegionForestSettingsSparse() const;
        NoiseSettings getRegionResourceSettings() const;
};
}

extern iso::WorldData world_settings;

#endif