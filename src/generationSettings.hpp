#ifndef _WORLD_SETTINGS_HPP_
#define _WORLD_SETTINGS_HPP_

#include "noiseSettings.hpp"

#include <SFML/System/Vector2.hpp>
#include <map>

namespace iso {    
// TODO:
// - Add more settings customisable settings.
// - Settings: Biome generation settings.
// - (?) Maybe make this class hold region data, that are currently stored by the SimulationManager class.

class WorldData {
    private:
        // World settings.

        int world_size;            // Side of the world area - x means world size is x * x.
        int world_margin_poles;    
        int world_margin_island;   // Distance between the edge of the world and the island.
        int world_river_quantity;  // Number of rivers that generation algorithm will attempt to generate.
        int world_river_scan_size; // Side of the area scanned by river generation algorithm - x means area size is (2x + 1)(2x + 1). 
        float world_noise_terrain; // From this value terrain will generate - has to be between 0 and 1.
        float world_noise_forest;  // From this value forests will generate - has to be between 0 and 1.
        int world_panel_size;
        
        // Region settings.
        
        int region_size;                 // Side of the region area - x means region size is x * x.
        sf::Vector2f region_tile_size;   
        sf::Vector2f region_tile_offset; // By how many tiles is the region offset.
    
        float resource_flint_chance; // Chance to generate flint in a region (0.00 - 1.00).
        float resource_stone_chance; // Chance to generate stone in a region (0.00 - 1.00).

        int resource_flint_radius; // How big the flint resource patch should be.
        int resource_stone_radius; // How big the stone resource patch should be.

        float tree_noise_minimum; // Smallest noise value acceptable to generate a tree tile (0.00 - 1.00).
        float flint_noise_minimum; // Smallest noise value acceptable to generate a flint tile (0.00 - 1.00).
        float stone_noise_minimum; // Smallest noise value acceptable to generate a stone tile (0.00 - 1.00).

        // Game data.

        int player_quantity; // Number of players in the game.

        // Noise.

        NoiseSettings world_noise;
        NoiseSettings world_moisture;
        NoiseSettings world_gradient;
        NoiseSettings world_forest;

        NoiseSettings region_forest_dense;
        NoiseSettings region_forest_sparse;
        NoiseSettings region_resource;

    private:
        // This should only be called once in the constructor.
        // It is a function only for readability.
        void loadSettingsFromFile();
    
    public:
        WorldData();
        ~WorldData();

        // Worldmap API

        bool inWorldBounds(int index) const;
        int getWorldSize()            const;
        int getWorldWidth()           const;
        int getWorldMargin()          const;
        int getWorldMarginPolar()     const;
        int getWorldRiverQuantity()   const;
        int getWorldRiverScan()       const;
        float getWorldTerrainMin()    const;
        float getWorldForestMin()     const; 
        int calculateWorldIndex(int x, int y)    const;
        NoiseSettings getWorldNoiseSettings()    const;
        NoiseSettings getWorldMoistureSettings() const;
        NoiseSettings getWorldGradientSettings() const;
        NoiseSettings getWorldForestSettings()   const;


        // Regionmap API

        int getRegionSize()    const;
        int getRegionWidth()   const;
        float getRegionFlintChance() const;
        float getRegionStoneChance() const;
        int getRegionFlintRadius()   const;
        int getRegionStoneRadius()   const;
        float getRegionTreeMin()     const;
        float getRegionFlintMin()    const;
        float getRegionStoneMin()    const;

        bool inRegionBounds(int index) const;
        bool inRegionBounds(sf::Vector2i grid_position) const;
        int calculateRegionIndex(int x, int y)        const;
        NoiseSettings getRegionForestSettingsDense()  const;
        NoiseSettings getRegionForestSettingsSparse() const;
        NoiseSettings getRegionResourceSettings()     const;

        // Miscellaneous

        int panelSize()           const;
        sf::Vector2f tileSize()   const;
        sf::Vector2f tileOffset() const;
        int getPlayerQuantity()   const;
};
}

extern iso::WorldData world_settings;

#endif