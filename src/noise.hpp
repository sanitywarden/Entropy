#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>

// y = y1 + ((x - x1) / (x2 - x1)) * (y2 - y1)

namespace iso {
    namespace wgn {
        class NoiseAlgorithms {
            private:
                double fade(double t) const;
                double lerp(double t, double a, double b) const;
                double grad(int hash, double x, double y, double z) const;
                void generatePermutationVector(unsigned int seed);
            
            private:
                unsigned int latest_seed;
                std::vector <int> permutation_vector; 

            public:
                NoiseAlgorithms();
                NoiseAlgorithms(unsigned int seed);
                ~NoiseAlgorithms();

                unsigned int getCurrentSeed() const;
                double noise(double x, double y, double z) const;        
        };
    }
}