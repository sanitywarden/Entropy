#ifndef _BUILDING_WOODCUTTER_
#define _BUILDING_WOODCUTTER_

#include "building.hpp"

namespace iso {
    class Woodcutter : public Building {
        private:
            
        public:
            Woodcutter();
            ~Woodcutter();

            void update(GameObject* object, int building_index) override;
            bool isBuildingResourceTile(GameObject* object, int index) const override;
    };
}

#endif