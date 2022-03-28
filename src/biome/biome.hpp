#ifndef _BIOME_HPP_
#define _BIOME_HPP_

#include "colours.hpp"

#include <string>

namespace iso {
    class Biome {
        public:
            std::string biome_name;   // Name used for biome recognition. Human readable.
            sf::Color   biome_colour; // Colour of the biome. Used for colouring worldmap textures.

            Biome();
            Biome(std::string name, sf::Color colour);
            ~Biome();

            bool operator== (const Biome& biome) const;

            virtual std::string getTile() const { return "default"; }
            virtual std::string getTree() const { return "default"; }
    };
}

#endif