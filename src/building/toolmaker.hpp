#ifndef _BUILDING_TOOLMAKER_HPP_
#define _BUILDING_TOOLMAKER_HPP_

#include "building.hpp"

namespace iso {
    class Toolmaker : public Building {
        private:

        public:
            Toolmaker();
            ~Toolmaker();

            void update(GameObject* object, int building_index) override;
    };
}

#endif