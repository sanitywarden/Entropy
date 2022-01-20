#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "gameObject.hpp"
#include "region.hpp"

#include <vector>

namespace iso {
    class Pawn : public GameObject {
        public:
            std::vector <int> path;
            Region*           region;
            int               current_index;

        public:
            Pawn();
            Pawn(Region* region);
            ~Pawn();

            const std::vector <int>& getAStarPath();
            bool                     hasPath();
            void                     nextMove();
    };
}

#endif