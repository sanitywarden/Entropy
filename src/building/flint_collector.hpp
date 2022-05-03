#ifndef _BUILDING_FLINT_COLLECTOR_HPP_
#define _BUILDING_FLINT_COLLECTOR_HPP_

#include "building.hpp"

namespace iso {
    class FlintCollector : public Building {
        private:

        public:
            FlintCollector();
            ~FlintCollector();

            void update(GameObject* object, int building_index) override;
    };
}

#endif