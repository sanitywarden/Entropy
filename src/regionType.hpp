#ifndef _REGION_TYPE_HPP_
#define _REGION_TYPE_HPP_

#include <string>
#include <map>

namespace iso {
    class RegionType {
        private:
            std::map <std::string, bool> type;
            
            bool is_type(std::string id) const;
            void set_type(std::string id, bool value);

        public:
            RegionType();
            ~RegionType();

            bool is_terrain() const;
            bool is_ocean()   const;
            bool is_river()   const;
            bool is_coast()   const;
            bool is_forest()  const;
            bool is_lake()    const;
            bool is_water()   const;

            void set_terrain();
            void set_ocean();
            void set_river();
            void set_coast();
            void set_forest();
            void set_lake();

            void unset_coast();
    };
}

#endif