#pragma once

#include "gameObject.hpp"

#include <vector>
#include <string>

namespace iso {
struct BiomeData {
    std::string filename;    // Path to the definition file of the biome.
    std::string name;        // Human readable name.
    std::string id;          // Code id. Used for texture naming. 
    std::string description; // Human readable description.
    sf::Color   colour_wmap; // Colour of the region in the worldmap view.
    sf::Color   colour_rmap; // Colour of the tiles in the regionmap view.
    std::string temperature; // Temperature (COLD, WARM, TROPICAL, HOT).
    std::string moisture;    // Moisture    (DRY, NORMAL, MOIST, TROPICAL).
    std::string latitude;    // Latitude    (POLAR, SUBPOLAR, BETWEEN, EQUATOR)
    std::string elevation;   // Elevation   (FLAT, HILLS, MOUNTAIN)
    std::string forest_texture_worldmap; // Texture of the forest in the worldmap view.
    std::vector <std::string> tiles;     // Terrain tiles of which the region consists.
    std::vector <std::string> trees;     // Trees of which the forest consists.
    bool can_be_forest;                  // Can generate a forest.
};

class Biome {
    protected:
        BiomeData data;
        
    public:
        Biome();
        Biome(const Biome& biome);
        Biome(const BiomeData& data);
        ~Biome();

        const std::string& getDefinitionFilename()    const;
        const std::string& getBiomeName()             const;
        const std::string& getBiomeId()               const;
        const std::string& getBiomeDescription()      const;
        const sf::Color    getBiomeWorldmapColour()   const;
        const sf::Color    getBiomeRegionmapColour()  const;
        const std::string& getBiomeTemperature()      const;
        const std::string& getBiomeMoisture()         const;
        const std::string& getBiomeLatitude()         const;
        const std::string& getBiomeElevation()        const;
        const std::string& getWorldmapForestTexture() const;
        const std::vector <std::string>& getBiomeTileList() const;
        const std::vector <std::string>& getBiomeTreeList() const;
        const std::string& getRandomTile() const;
        const std::string& getRandomTree() const;
        bool               canBeForest()   const;
};
}

extern std::vector <iso::Biome> biomes;