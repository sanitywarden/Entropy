#ifndef _BUILDING_WELL_
#define _BUILDING_WELL_

#include "building.hpp"

namespace iso {
    class Well : public Building {
        private:

        public:
            Well();
            ~Well();

            void update(GameObject* object, int building_index) override;
    };
}

#endif