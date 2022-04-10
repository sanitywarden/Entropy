#ifndef _NOISE_SETTINGS_HPP_
#define _NOISE_SETTINGS_HPP_

#include <SFML/System/Vector2.hpp>

namespace iso {
struct NoiseSettings {
    NoiseSettings()
        : size(sf::Vector2f(0, 0)), octaves(0), persistence(0), bias(0), multiplier(0) {}

    NoiseSettings(sf::Vector2f size, int octaves, int persistence, int bias, float multiplier)  
        : size(size), octaves(octaves), persistence(persistence), bias(bias), multiplier(multiplier) {}

    NoiseSettings(int width, int octaves, int persistence, int bias, float multiplier)
        : size(sf::Vector2f(width, width)), octaves(octaves), persistence(persistence), bias(bias), multiplier(multiplier) {}

    sf::Vector2f size;
    int          octaves;
    int          persistence;
    int          bias;
    float        multiplier;
};
}

#endif