#ifndef _GLOBAL_UTILITIES_HPP_
#define _GLOBAL_UTILITIES_HPP_

#include <string>

#include <SFML/System/Vector2.hpp>

namespace iso {
    bool startsWith(const std::string& str, const std::string& phrase);
    bool containsWord(const std::string& str, const std::string& phrase);
    std::string toLower(const std::string& str);

    bool inCircle(const sf::Vector2f point, const sf::Vector2f centre, const int radius);
}

#endif