#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "gameObject.hpp"

#include <vector>

namespace iso {
    class Unit : public GameObject {
        private:
            int id;

        public:
            static int        id_count;
            std::vector <int> path;
            int               current_index;
            int               goal;

        public:
            Unit();
            ~Unit();

            int                      getID() const;
            void                     setNewPath(std::vector <int> path);
            const std::vector <int>& getAStarPath();
            int                      getNextMove();
            bool                     hasPath() const;
    };
}

#endif