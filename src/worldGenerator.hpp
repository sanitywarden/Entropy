#ifndef _WORLD_GENERATOR_HPP_
#define _WORLD_GENERATOR_HPP_

#include "generationSettings.hpp"
#include "entropy/entropy.hpp"
#include "region.hpp"
#include "biome.hpp"

#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <cmath>

namespace iso {
    typedef std::vector <float> NoiseContainer;
    
    class WorldGenerator {
        private:
            Region m_region;
            Tile   m_tile;

            entropy::resourceManager* resource;
            std::vector <float> m_gradient; 
        
        private:
            void generateNoise(NoiseSettings& settings, std::vector<float>& container);
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

            // std::string getRiverTile(int index) const;
            std::string getRiverTileVariation(const std::string& id) const;

            std::string createColouredTexture(const std::string& id, const std::string& save_as, const sf::Color colour_main, const sf::Color colour_secondary) const;

            // Regionmap API.
            
            // std::string getBiomeSpecificTileAtlas(const Biome& biome) const;
    
            void regionGenerateHeight(int region_index);
            void regionGenerateRiver (int region_index);
            void regionGenerateForest(int region_index);

            std::string getBiomeTileTextureName (Biome biome); 
            std::string getTreeTextureNameRegion(Biome biome);

        public:
            WorldGenerator();
            WorldGenerator(entropy::resourceManager* resource, GenerationSettings& settings);
            ~WorldGenerator();
        
            void generateWorld();
            void generateRegion(int index, Region& region);
    
            sf::Vector2f tilePositionScreen(int x, int y);
            sf::Vector2f tilePositionScreen(sf::Vector2i grid_position);
            sf::Vector2f tilePositionScreen(sf::Vector2f grid_position);
            
            int getTileIndex(sf::Vector2f mouse_position, Region& region);
            std::string getTilePixelColour(sf::Vector2i);

            GenerationSettings settings;

            bool is_arctic       (int index) const;
            bool is_ocean        (int index) const;
            bool is_sea          (int index) const;
            bool is_tropical     (int index) const;
            bool is_mediterranean(int index) const;
            bool is_temperate    (int index) const;
            bool is_continental  (int index) const;
            bool is_tundra       (int index) const;
            bool is_desert       (int index) const;
            
            bool is_terrain      (int index) const;
            bool is_coast        (int index) const;

            int getWorldSize()  const;
            int getRegionSize() const;
            int rCalculateIndex(int x, int y) const;
            int wCalculateIndex(int x, int y) const;
        public:
            std::vector <Region>          world_map; 
            std::map    <int, GameObject> forests;
            std::map    <int, GameObject> rivers;
    };
}

#endif