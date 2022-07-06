#pragma once

#include "resource.hpp"

namespace iso {
    class Flint : public Resource {
        private:

        public:
            Flint();
            ~Flint();

            bool isGenerationSpotValid(GameObject* object, int index) const override;
            void placeResource(GameObject* object, int index) const override;
    };
}