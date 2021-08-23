#ifndef _WORLD_GENERATOR_HPP_
#define _WORLD_GENERATOR_HPP_

#include "panel.hpp"
#include "worldSettings.hpp"
#include "entropy/entropy.hpp"

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

            float generateNoise(int&, int&);
            void generatePerlinNoise();
            void generateCircularGradient();
            void generatePoles();
            void generateRivers();

        public:
            worldGenerator();
            worldGenerator(WorldSettings& settings, entropy::Entropy* engine);
            ~worldGenerator();

            std::vector <Panel> world_map;
    };
}

#endif