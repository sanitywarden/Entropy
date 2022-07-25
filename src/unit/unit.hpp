#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "gameObject.hpp"

#include <vector>
#include <string>

namespace iso {
    enum class UnitType {
        UNIT_SETTLER,
        UNIT_WARRIOR,

        UNITTYPES_TOTAL_QUANTITY
    };
    
    class Unit : public GameObject {
        private:
            int id;

        public:
            static int        id_count;
            int               owner_id;
            std::vector <int> path;
            int               current_index;
            int               goal;
            std::string       unit_name;     // Human readable unit name. 
            UnitType          type;

        public:
            Unit();
            Unit(std::string unit_name);
            ~Unit();

            bool operator== (const Unit& unit) const;
            bool operator!= (const Unit& unit) const;

            int                      getID() const;
            std::string              getUnitName() const;
            void                     setNewPath(std::vector <int> path);
            const std::vector <int>& getAStarPath();
            int                      getNextMove();
            bool                     hasPath() const;
            bool                     isSameType(UnitType type) const;
    };

    const Unit UNIT_SETTLER("Settler");
}

#endif