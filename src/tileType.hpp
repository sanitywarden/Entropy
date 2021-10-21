#ifndef _TILE_TYPE_HPP_
#define _TILE_TYPE_HPP_

#include <bitset>

// What does each bit mean?
// 1 - 1 terrain, 0 water.
// 2 - 1 river, 0 ocean.

namespace iso {
    typedef std::bitset <8> bitmask;
        
    // Bit representation of the type of a tile.
    // Some of these bits are mutually exclusive.
    class TileType {
        private:
            bitmask m_type;

            bool is_type(char bit);
            void set_type(char bit, bool value = true);

        public: 
            bitmask& tileType();

            bool is_terrain();
            bool is_water();   // Returns true if the tile is of type river or ocean.
            bool is_river();
            bool is_ocean();

            void set_terrain();
            void set_water();
            void set_river();
            void set_ocean();
    };     
}

#endif