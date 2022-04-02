#ifndef _BUILDING_QUARRY_
#define _BUILDING_QUARRY_

#include "building.hpp"

namespace iso {
    class Quarry : public Building {
        private:

        public:
            Quarry();
            ~Quarry();
            
            void update(GameObject* object, int building_index) override;
    };
}

#endif