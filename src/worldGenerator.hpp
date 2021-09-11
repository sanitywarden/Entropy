#ifndef _WORLD_GENERATOR_HPP_
#define _WORLD_GENERATOR_HPP_

#include "panel.hpp"
#include "worldSettings.hpp"
#include "entropy/entropy.hpp"
#include "region.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <iostream>
#include <cmath>

namespace iso {
    class worldGenerator {
        private:
            WorldSettings m_world_settings;
            Panel m_panel;

            entropy::Entropy *m_engine;

            std::string m_log_prefix;

            std::vector <float> m_noise;
            std::vector <float> m_gradient;
        private:
            void generateWorld();

            void generateNoiseMap();
            void generateCircularGradient();
            void generatePoles();
            void generateRivers();
            void generateLatititude();
            void generateTemperature();
            void generateMoistureMap();
            void assignBiome();

        public:
            worldGenerator();
            worldGenerator(WorldSettings& settings, entropy::Entropy* engine);
            ~worldGenerator();

            std::vector <Panel>  world_map;  // Collection of all the panels on the worldmap.
            std::vector <Region> region_map; // Collection of all the region data classes used by Regionmap gamestate.
    };
}

#endif