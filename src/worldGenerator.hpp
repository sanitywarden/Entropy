#pragma once

#include "entropy/entropy.hpp"
#include "texturizer.hpp"
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

            resourceManager*     resource;
            Texturizer*          texturizer;
            wgn::NoiseAlgorithms noise;     
            std::vector <float> m_gradient; 
        
        private:
            bool is_biome(int index, Biome biome) const;            

            // Initial worldmap generation.
            
            void worldmapGenerateClimate();
            void worldmapAssignBiome();
            void worldmapGenerateForests();
            void worldmapGenerateRivers();

            // Worldmap API.
            
            std::string getWorldmapTile(int index) const;
            std::string getTileVariation(const std::string& id) const;
            std::string createBiomeSpecificTexture(const std::string& id, Biome biome) const;
            std::string getWorldmapTreeTextureName(const Biome& biome) const;
            std::string getRiverTileVariation(const std::string& id) const;
            std::string extractBaseTexture(const std::string& id, const Biome& biome) const;

            // Regionmap API.            

            void generateResourcePatch(Region& region, const Resource& resource);

        public:
            WorldGenerator();
            WorldGenerator(resourceManager* resource, Texturizer* texturizer);
            ~WorldGenerator();

            void generateWorld();
            void generateRegion(int region_index);
    
            void generateNoise(NoiseSettings& settings, NoiseContainer& container);
            void generateChunkNoise(NoiseContainer& container, sf::Vector2i chunk_grid);

            sf::Vector3f tilePositionScreen(int x, int y);
            sf::Vector3f tilePositionScreen(sf::Vector2i grid_position);
            sf::Vector3f tilePositionScreen(sf::Vector2f grid_position);
            
            bool is_ocean        (int index) const;
            bool is_tropical     (int index) const;
            bool is_mediterranean(int index) const;
            bool is_temperate    (int index) const;
            bool is_continental  (int index) const;
            bool is_tundra       (int index) const;
            
            bool is_terrain      (int index) const;
            bool is_coast        (int index) const;
            bool is_lake         (int index) const;
            bool is_river        (int index) const;

        public:
            std::vector <Region>          world_map; 
            std::map    <int, GameObject> forests;
            std::map    <int, GameObject> rivers;
            std::map    <int, GameObject> lakes;
    };
}