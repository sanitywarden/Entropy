#ifndef _BIOME_HPP_
#define _BIOME_HPP_

#include <string>

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

            Biome();
            Biome(std::string name, Climate climate);
            ~Biome();
    };

    const Biome BIOME_TROPICAL      ("Tropical"     , Climate::TROPICAL);
    const Biome BIOME_MEADITERRANEAN("Mediterranean", Climate::MEDITERRANEAN);
    const Biome BIOME_TEMPERATE     ("Temperate"    , Climate::TEMPERATE);
    const Biome BIOME_CONTINENTAL   ("Continental"  , Climate::CONTINENTAL);
    const Biome BIOME_OCEAN         ("Ocean"        , Climate::OCEAN);
    const Biome BIOME_SEA           ("Sea"          , Climate::SEA);
    const Biome BIOME_ARCTIC        ("Arctic"       , Climate::ARCTIC);
    const Biome BIOME_TUNDRA        ("Tundra"       , Climate::TUNDRA);
    const Biome BIOME_DESERT        ("Desert"       , Climate::DESERT);
}

#endif