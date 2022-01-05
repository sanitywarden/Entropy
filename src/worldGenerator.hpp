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
    class worldGenerator {
        private:
            Region m_region;
            Tile   m_tile;

            entropy::resourceManager* resource;

            std::string m_log_prefix;
            bool generated;

            std::vector <float> m_noise;    
            std::vector <float> m_gradient; 
            std::vector <float> m_tree_noise;

        private:
            void generateNoise(noiseSettings& settings, std::vector<float>& container);
            bool is_biome(int index, Biome biome);            

            void generateNoiseMap();
            void generateCircularGradient();
            void generatePoles();
            void generateLatititude();
            void generateTemperature();
            void generateMoistureMap();
            void assignBiome();
            void generateRivers();
            void generateForests();

            void regionGenerateHeight(int region_index);
            void regionGenerateRiver (int region_index);
            void regionGenerateForest(int region_index);

            std::string getBiomeTileTextureName (Biome biome); 
            std::string getTreeTextureNameWorld (Biome biome);
            std::string getTreeTextureNameRegion(Biome biome);

        public:
            worldGenerator();
            worldGenerator(entropy::resourceManager* resource, GenerationSettings& settings);
            ~worldGenerator();
        
            void generateWorld();
            void generateRegion(int index, Region& region);
    
            sf::Vector2f tilePositionScreen(int x, int y);
            sf::Vector2f tilePositionScreen(sf::Vector2i grid_position);
            sf::Vector2f tilePositionScreen(sf::Vector2f grid_position);
            
            int getTileIndex(sf::Vector2f mouse_position, Region& region);
            std::string getTilePixelColour(sf::Vector2i);

            GenerationSettings settings;

            bool is_arctic       (int index);
            bool is_ocean        (int index);
            bool is_sea          (int index);
            bool is_tropical     (int index);
            bool is_mediterranean(int index);
            bool is_temperate    (int index);
            bool is_continental  (int index);
            bool is_tundra       (int index);
            bool is_desert       (int index);

            int getWorldSize();
            int getRegionSize();
            int rCalculateIndex(int x, int y);
            int wCalculateIndex(int x, int y);
        public:
            std::vector <Region> world_map; 
    };
}

#endif