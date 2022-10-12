#pragma once

#include <string>
#include <map>

namespace iso {    
    class TileType {
        private:
            std::map <std::string, bool> type;

            void set_type(std::string id, bool value);
            
        public: 
            TileType();
            ~TileType();

            bool is_type(const std::string& id) const;
            bool is_terrain() const;
            bool is_water()   const;
            bool is_river()   const;
            bool is_ocean()   const;
            bool is_coast()   const;

            void set_terrain();
            void set_river();
            void set_ocean();
            void set_coast();
    };     
}