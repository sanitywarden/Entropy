#ifndef _REGION_TYPE_HPP_
#define _REGION_TYPE_HPP_

#include <bitset>

// 1 - 1 terrain, 0 water.
// 2 - 1 coast, 0 not coast.
// 3 - 1 river, 0 not river.
// 4 - 1 arctic, 0 not arctic.
// 5 - 1 forest, 0 not forest.

namespace iso {
    typedef std::bitset <8> bitmask;

    class RegionType {
        private:
            bitmask m_type;
            
            bool is_type(char bit);
            void set_type(char bit, bool value = true);

        public:
            bitmask& regionType();

            bool is_terrain();
            bool is_ocean();
            bool is_river();
            bool is_coast();
            bool is_arctic();
            bool is_forest();

            void set_terrain();
            void set_ocean();
            void set_river();
            void set_coast();
            void set_arctic();
            void set_forest();
    };
}

#endif