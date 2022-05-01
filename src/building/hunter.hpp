#ifndef _BUILDING_HUNTER_
#define _BUILDING_HUNTER_

#include "building.hpp"

namespace iso {
    class Hunter : public Building {
        private:
            
        public:
            Hunter();
            ~Hunter();

            void update(GameObject* object, int building_index) override;
    };
}

#endif