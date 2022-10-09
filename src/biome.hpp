#pragma once

#include "types.hpp"
#include "gameObject.hpp"

#include <vector>
#include <string>

namespace iso {
struct BiomeData {
    std::string  filename;    // Path to the definition file of the biome.
    std::string  name;        // Human readable name.
    std::string  id;          // Code id. Used for texture naming. 
    std::string  description; // Human readable description.
    core::Colour colour_wmap; // Colour of the region in the worldmap view.
    std::string  temperature; // Temperature (COLD, WARM, TROPICAL, HOT).
    std::string  moisture;    // Moisture    (DRY, NORMAL, MOIST, TROPICAL).
    std::string  forest_texture_worldmap; // Texture of the forest in the worldmap view.
    std::vector  <std::string> tiles;     // Terrain tiles of which the region consists.
    std::vector  <std::string> trees;     // Trees of which the forest consists.
    bool         can_be_forest;           // Can generate a forest.
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
        const core::Colour getBiomeWorldmapColour()   const;
        const std::string& getBiomeTemperature()      const;
        const std::string& getBiomeMoisture()         const;
        const std::string& getWorldmapForestTexture() const;
        const std::vector <std::string>& getBiomeTileList() const;
        const std::vector <std::string>& getBiomeTreeList() const;
        const std::string& getRandomTile() const;
        const std::string& getRandomTree() const;
        bool               canBeForest()   const;

        void setBiomeName(const std::string& name);
        void setBiomeId(const std::string& id);
        void setBiomeDescription(const std::string& description);
        void setBiomeWorldmapColour(core::Colour colour);
        void setBiomeTemperature(const std::string& temperature);
        void setBiomeMoisture(const std::string& moisture);
        void setWorldmapForestTexture(const std::string& texture_name);
        void setBiomeTileList(std::vector <std::string> list);
        void setBiomeTreeList(std::vector <std::string> list);

        bool operator== (const Biome& biome) const;
};
}

extern std::vector <iso::Biome> BIOME_TABLE;
