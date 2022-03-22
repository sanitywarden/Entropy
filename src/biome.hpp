#ifndef _BIOME_HPP_
#define _BIOME_HPP_

#include <string>
#include <SFML/Graphics/Color.hpp>

#include "colours.hpp"

namespace iso {
    enum class Climate {
        BIOMELESS,
        TROPICAL,
        MEDITERRANEAN,
        TEMPERATE,   // The "warm" grasslands.
        CONTINENTAL, // The "cold" grasslands.
        OCEAN,
        SEA,
        ARCTIC,
        TUNDRA,
        DESERT,
    };

    class Biome {
        public:
            std::string biome_name;
            Climate     biome_climate;
            sf::Color   biome_colour;

            Biome();
            Biome(std::string name, sf::Color biome_colour, Climate climate);
            ~Biome();

            bool operator== (const Biome& biome) const;
    };

    const Biome BIOME_TROPICAL      ("Tropical"     , COLOUR_GREEN_TROPICAL     , Climate::TROPICAL);
    const Biome BIOME_MEADITERRANEAN("Mediterranean", COLOUR_GREEN_MEDITERRANEAN, Climate::MEDITERRANEAN);
    const Biome BIOME_TEMPERATE     ("Temperate"    , COLOUR_GREEN_TEMPERATE    , Climate::TEMPERATE);
    const Biome BIOME_CONTINENTAL   ("Continental"  , COLOUR_GREEN_CONTINENTAL  , Climate::CONTINENTAL);
    const Biome BIOME_OCEAN         ("Ocean"        , COLOUR_BLUE_OCEAN         , Climate::OCEAN);
    const Biome BIOME_SEA           ("Sea"          , COLOUR_BLUE_OCEAN         , Climate::SEA);
    const Biome BIOME_TUNDRA        ("Tundra"       , COLOUR_BROWN_TUNDRA       , Climate::TUNDRA);
    const Biome BIOME_ARCTIC        ("Arctic"       , COLOUR_WHITE              , Climate::ARCTIC);
    const Biome BIOME_DESERT        ("Desert"       , COLOUR_YELLOW             , Climate::DESERT);
}

#endif