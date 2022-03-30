#ifndef _TILE_TYPE_HPP_
#define _TILE_TYPE_HPP_

#include <string>
#include <map>

namespace iso {    
    class TileType {
        private:
            std::map <std::string, bool> type;

            bool is_type(std::string id) const;
            void set_type(std::string id, bool value);
            
        public: 
            TileType();
            ~TileType();

            bool is_terrain() const;
            bool is_water()   const; // Returns true if the tile is of type river or ocean.
            bool is_river()   const;
            bool is_ocean()   const;

            void set_terrain();
            void set_river();
            void set_ocean();
    };     
}

#endif