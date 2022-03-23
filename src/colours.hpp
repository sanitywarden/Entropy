#ifndef _COLOURS_HPP_
#define _COLOURS_HPP_

#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace iso {
    const sf::Color COLOUR_TRANSPARENT(0, 0, 0, 0);
    const sf::Color COLOUR_WHITE      (255, 255, 255, 255);
    const sf::Color COLOUR_BLACK      (0, 0, 0, 255);

    const sf::Color COLOUR_WHITE_TRANSPARENT_HALF   (255, 255, 255, 127);
    const sf::Color COLOUR_WHITE_TRANSPARENT_QUARTER(255, 255, 255, 63);

    // Biome specific colours.

    const sf::Color COLOUR_GREEN_TROPICAL     (79, 154, 22); 
    const sf::Color COLOUR_GREEN_MEDITERRANEAN(132, 177, 29);
    const sf::Color COLOUR_GREEN_TEMPERATE    (46, 131, 31);
    const sf::Color COLOUR_GREEN_CONTINENTAL  (24, 77, 14);
    const sf::Color COLOUR_BROWN_TUNDRA       (77, 77, 30);
    const sf::Color COLOUR_BLUE_OCEAN         (67, 102, 228);
    
    const sf::Color COLOUR_BLUE_RIVER(47, 154, 229);

    const sf::Color COLOUR_RED    (229, 20, 0, 255);
    const sf::Color COLOUR_BLUE   (0, 80, 239, 255);
    const sf::Color COLOUR_GREEN  (0, 138, 0, 255);
    const sf::Color COLOUR_PURPLE (106, 0, 255, 255);
    const sf::Color COLOUR_YELLOW (227, 200, 0, 255);
    const sf::Color COLOUR_CYAN   (27, 161, 226, 255);
    

    const std::vector <sf::Color> TEAM_COLOURS = {
        COLOUR_RED, 
        COLOUR_BLUE, 
        COLOUR_GREEN,
        COLOUR_PURPLE,
        COLOUR_YELLOW,
        COLOUR_CYAN
    };
}

#endif