#pragma once

#include "region.hpp"
#include "biome.hpp"
#include "noiseSettings.hpp"
#include "noise.hpp"

#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>

namespace iso {
    typedef std::vector <float> NoiseContainer;
    
    class WorldGenerator {
        private:
            Region m_region;
            Tile   m_tile;

            wgn::NoiseAlgorithms noise;     
            std::vector <float>  m_gradient; 
        
        private:
            bool is_biome(int index, Biome biome) const;            

            // Initial worldmap generation.

            void generateWorld();
            void worldmapGenerateClimate();
            void worldmapAssignBiome();
            void worldmapGenerateForests();
            void worldmapGenerateRivers();
            void spawnPlayers();

            // Worldmap API.
            
            // This function returns the name of the tile type under certain index.
            // Exact result is the name of a worldmap tile without type ID.
            // For example: "panel_island" instead of "panel_island_0";
            std::string getWorldmapTile(int index) const;
            // const Biome& getBiome(float temperature, float moisture, float elevation) const;

            // Get a tile variation from "./res/worldmap/panel_atlas.png".
            std::string getTileVariation(const std::string& id) const;

            // Use the texture templates from "./res/worldmap/panel_atlas.png" to create their biome specific variations.
            std::string createBiomeSpecificTexture(const std::string& id, Biome biome) const;
            
            std::string getRiverTileVariation(const std::string& id) const;
            std::string extractBaseTexture(const std::string& id, const Biome& biome) const;

            // Regionmap API.            

            void generateResourcePatch(Region& region, const Resource& resource);

        public:
            WorldGenerator();
            ~WorldGenerator();

            void generate();            
            void generateRegion(int region_index);
    
            void generateNoise(NoiseSettings& settings, NoiseContainer& container);
            void generateChunkNoise(NoiseContainer& container, sf::Vector2i chunk_grid);

            core::Vector3i tilePositionScreen(int x, int y);
            core::Vector3i tilePositionScreen(core::Vector2i grid_position);
            core::Vector3i tilePositionScreen(core::Vector2f grid_position);
            
            bool is_ocean        (int index) const;
            bool is_tropical     (int index) const;
            bool is_mediterranean(int index) const;
            bool is_temperate    (int index) const;
            bool is_continental  (int index) const;
            bool is_tundra       (int index) const;
            
            bool is_terrain(int index) const;
            bool is_coast  (int index) const;
            bool is_lake   (int index) const;
            bool is_river  (int index) const;
    };
}