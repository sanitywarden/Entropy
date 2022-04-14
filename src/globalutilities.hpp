#ifndef _GLOBAL_UTILITIES_HPP_
#define _GLOBAL_UTILITIES_HPP_

#include <string>

#include <SFML/System/Vector2.hpp>

namespace iso {
    bool startsWith(const std::string& str, const std::string& phrase);
    bool containsWord(const std::string& str, const std::string& phrase);
    std::string readAfter(const std::string& str, char from, char to);
    std::string readBefore(const std::string& str, char to);
    std::string read(const std::string& str, int incl_from, int to);
    std::string toLower(const std::string& str);
    size_t find(const std::string& str, const std::string& phrase);

    bool inCircle(const sf::Vector2f point, const sf::Vector2f centre, const int radius);
}

#endif