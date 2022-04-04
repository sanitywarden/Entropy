#ifndef _BIOME_HPP_
#define _BIOME_HPP_

#include "colours.hpp"

#include <vector>
#include <string>

namespace iso {
    class Biome {
        public:
            std::string biome_name;   // Name used for biome recognition. Human readable.
            sf::Color   biome_colour; // Colour of the biome. Used for colouring worldmap textures.
            std::vector <std::string> biome_texture_tree;
            std::vector <std::string> biome_texture_tile;

            Biome();
            Biome(std::string name, sf::Color colour);
            Biome(std::string name, sf::Color colour, std::vector <std::string> tileset, std::vector <std::string> treeset);
            ~Biome();

            bool operator== (const Biome& biome) const;

            std::string getTile() const;
            std::string getTree() const;
    };

    static Biome BIOME_OCEAN("Ocean", COLOUR_BLUE_OCEAN);
    static Biome BIOME_TEMPERATE("Temperate", COLOUR_GREEN_TEMPERATE, 
        { "tile_grass_warm" }, 
        { "tile_tree_beech", "tile_tree_oak", "tile_tree_maple" });
    static Biome BIOME_CONTINENTAL("Continental", COLOUR_GREEN_CONTINENTAL, 
        { "tile_grass_cold" }, 
        { "tile_tree_spruce_1", "tile_tree_spruce_2", "tile_tree_pine" }); 
    static Biome BIOME_MEDITERRANEAN("Mediterranean", COLOUR_GREEN_MEDITERRANEAN, 
        { "tile_grass_subtropical" }, 
        { "tile_tree_cypress", "tile_tree_acacia" }); 
    static Biome BIOME_TROPICAL("Tropical", COLOUR_GREEN_TROPICAL, 
        { "tile_grass_tropical" }, 
        { "tile_tree_cypress", "tile_tree_acacia", "tile_tree_palm" });
    static Biome BIOME_TUNDRA("Tundra", COLOUR_BROWN_TUNDRA, 
        { "tile_tundra" }, 
        { "tile_tree_spruce_1", "tile_tree_spruce_2" });
}

#endif