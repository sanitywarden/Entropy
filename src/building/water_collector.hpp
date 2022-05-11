#ifndef _BUILDING_WATER_COLLECTOR_
#define _BUILDING_WATER_COLLECTOR_

#include "building.hpp"

namespace iso {
    class WaterCollector : public Building {
        private:

        public:
            WaterCollector();
            ~WaterCollector();

            void update(GameObject* object, int building_index) override;
    };
}

#endif