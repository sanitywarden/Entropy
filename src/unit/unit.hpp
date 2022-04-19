#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "gameObject.hpp"

#include <vector>
#include <string>

namespace iso {
    class Unit : public GameObject {
        private:
            int id;

        public:
            static int        id_count;
            int               owner_id;
            std::vector <int> path;
            int               current_index;
            int               goal;

        public:
            Unit();
            Unit(std::string unit_type);
            ~Unit();

            bool operator== (const Unit& unit) const;
            bool operator!= (const Unit& unit) const;

            int                      getID() const;
            void                     setNewPath(std::vector <int> path);
            const std::vector <int>& getAStarPath();
            int                      getNextMove();
            bool                     hasPath() const;
    };
}

#endif