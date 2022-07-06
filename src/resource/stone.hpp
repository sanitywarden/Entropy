#pragma once

#include "resource.hpp"

namespace iso {
    class Stone : public Resource {
        private:

        public:
            Stone();
            ~Stone();

            bool isGenerationSpotValid(GameObject* object, int index) const override;
            void placeResource(GameObject* object, int index) const override;
    };
}