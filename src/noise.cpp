#include "noise.hpp"

#include <random>
#include <cmath>
#include <iostream>
#include <algorithm>

// Default constructor generates the permutation table based on a random seed.
iso::wgn::NoiseAlgorithms::NoiseAlgorithms() {
    std::srand(time(0));
    this->latest_seed = std::abs(std::rand());
    this->generatePermutationVector(this->latest_seed);
}

// Argument contructor creates the permutation table based on provided seed.
iso::wgn::NoiseAlgorithms::NoiseAlgorithms(unsigned int seed) {
    this->latest_seed = seed;
    this->generatePermutationVector(seed);
}

iso::wgn::NoiseAlgorithms::~NoiseAlgorithms()
{}

void iso::wgn::NoiseAlgorithms::generatePermutationVector(unsigned int seed) {
    this->permutation_vector = std::vector <int> (256);

    // Fill with values from 0 to 255.
    std::iota(this->permutation_vector.begin(), this->permutation_vector.end(), 0);

    // Shuffle the permutation vector based on the seed.
    std::default_random_engine engine(seed);
    std::shuffle(this->permutation_vector.begin(), this->permutation_vector.end(), engine);

    // Duplicate the permutation vector.
    this->permutation_vector.insert(this->permutation_vector.end(), this->permutation_vector.begin(), this->permutation_vector.end());
}

unsigned int iso::wgn::NoiseAlgorithms::getCurrentSeed() const {
    return this->latest_seed;
}

double iso::wgn::NoiseAlgorithms::fade(double t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double iso::wgn::NoiseAlgorithms::lerp(double t, double a, double b) const {
    return a + t * (b - a);
}

double iso::wgn::NoiseAlgorithms::grad(int hash, double x, double y, double z) const {
    int h = hash & 15;                      
    double u = h < 8 ? x : y;              
    double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

double iso::wgn::NoiseAlgorithms::noise(double x, double y, double z) const {
    int X = (int)std::floor(x) & 255;
    int Y = (int)std::floor(y) & 255;
    int Z = (int)std::floor(z) & 255;

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    double u = this->fade(x);
    double v = this->fade(y);
    double w = this->fade(z);

    int A  = this->permutation_vector[X] + Y;
    int AA = this->permutation_vector[A] + Z;
    int AB = this->permutation_vector[A + 1] + Z;
    int B  = this->permutation_vector[X + 1] + Y;
    int BA = this->permutation_vector[B] + Z;
    int BB = this->permutation_vector[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u,   grad(this->permutation_vector[AA  ], x  , y  , z   ),  
                                     grad(this->permutation_vector[BA  ], x-1, y  , z   )), 
                             lerp(u, grad(this->permutation_vector[AB  ], x  , y-1, z   ),  
                                     grad(this->permutation_vector[BB  ], x-1, y-1, z   ))),
                     lerp(v, lerp(u, grad(this->permutation_vector[AA+1], x  , y  , z-1 ),  
                                     grad(this->permutation_vector[BA+1], x-1, y  , z-1 )), 
                             lerp(u, grad(this->permutation_vector[AB+1], x  , y-1, z-1 ),
                                     grad(this->permutation_vector[BB+1], x-1, y-1, z-1 ))));
}
