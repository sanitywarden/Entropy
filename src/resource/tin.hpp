#pragma once

#include "resource.hpp"

namespace iso {
    class Tin : public Resource {
        private:
        public:
            Tin();
            ~Tin();

            bool isGenerationSpotValid(GameObject* object, int index) const;
            void placeResource(GameObject* object, int index) const;
    };
}