#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "gameObject.hpp"

#include <vector>

namespace iso {
    class Pawn : public GameObject {
        public:
            std::vector <int> path;
            int               current_index;

        public:
            Pawn();
            ~Pawn();

            void                     setNewPath(std::vector <int> path);
            const std::vector <int>& getAStarPath();
            int                      getNextMove();
            bool                     hasPath();
    };
}

#endif