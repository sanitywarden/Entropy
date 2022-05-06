#ifndef _BUILDING_FARM_
#define _BUILDING_FARM_

#include "building.hpp"

namespace iso {
    class Farmhouse : public Building {
        private:

        public:
            Farmhouse();
            ~Farmhouse();

            void update(GameObject* object, int building_index) override;
    };
}

#endif