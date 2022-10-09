#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "types.hpp"

namespace iso {
    bool startsWith(const std::string& str, const std::string& phrase);
    bool endsWith(const std::string& str, const std::string& phrase);
    bool containsWord(const std::string& str, const std::string& phrase);
    std::string readAfter(const std::string& str, const std::string& from);
    std::string readBefore(const std::string& str, const std::string& to);
    std::string read(const std::string& str, int incl_from, int to);
    std::string toLower(const std::string& str);
    std::string toHigher(const std::string& str);
    size_t find(const std::string& str, const std::string& phrase);
    std::string trimWhitespace(const std::string& str);
    bool inCircle(const sf::Vector2f point, const sf::Vector2f centre, const int radius);
    std::string asBool(int number);
    std::string asBool(bool condition);
    std::string capitalise(const std::string& str);
    // sf::Vector2i tileGridPosition(const sf::Texture& texture, sf::Vector2f tile_position);
    // sf::Vector2i tileGridPosition(int index);    
    core::Colour getTilePixelColour(core::Vector2i pixel); 
    int randomInclusiveBetween(int min, int max);
    core::Colour getRandomColour();
    void printError(const std::string& where, const std::string& message);
}
