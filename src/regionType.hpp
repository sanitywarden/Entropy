#ifndef _REGION_TYPE_HPP_
#define _REGION_TYPE_HPP_

#include <bitset>

// What does each bit mean?
// 1 - 1 terrain, 0 water.
// 2 - 1 coast, 0 not coast.
// 3 - 1 river, 0 not river.
// 4 - 1 forest, 0 not forest.

namespace iso {
    typedef std::bitset <8> bitmask;

    class RegionType {
        private:
            bitmask m_type;
            
            bool is_type(char bit) const;
            void set_type(char bit, bool value = true);

        public:
            bitmask& regionType();

            bool is_terrain() const;
            bool is_ocean()   const;
            bool is_river()   const;
            bool is_coast()   const;
            bool is_forest()  const;

            void set_terrain();
            void set_ocean();
            void set_river();
            void set_coast();
            void set_forest();
    };
}

#endif