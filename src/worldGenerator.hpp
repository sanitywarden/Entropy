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

            entropy::Entropy *m_engine;

            std::string m_log_prefix;

            std::vector <float> m_noise;    
            std::vector <float> m_gradient; 
            std::vector <float> m_tree_noise;

        private:
            void generateNoiseMap();
            void generateCircularGradient();
            void generatePoles();
            void generateLatititude();
            void generateTemperature();
            void generateMoistureMap();
            void assignBiome();
            void generateRivers();
            void generateForests();

            void generateNoise(noiseSettings& settings, std::vector<float>& container);

            bool is_biome(int index, Biome biome);            

            sf::Texture& getBiomeTileTexture(Biome biome); 

            sf::Texture& getTreeTextureWorld (Biome biome);
            sf::Texture& getTreeTextureRegion(Biome biome);

        public:
            worldGenerator();
            worldGenerator(WorldSettings& world_settings, RegionSettings& region_settings, entropy::Entropy* engine);
            ~worldGenerator();
        
            void generateWorld();
            void generateRegion(int index, Region& region);
    
            sf::Vector2f tilePositionScreen(int x, int y);
            sf::Vector2f tilePositionScreen(sf::Vector2i grid_position);
            sf::Vector2f tilePositionScreen(sf::Vector2f grid_position);
            
            int getTileIndex(sf::Vector2f mouse_position, Region& region);

            WorldSettings  world_settings;
            RegionSettings region_settings;

            bool is_coast  (int index);
            bool is_terrain(int index);
            bool is_forest (int index);
            bool is_arctic (int index);
            bool is_river  (int index);

            std::vector <Region> world_map; 
    };
}

#endif