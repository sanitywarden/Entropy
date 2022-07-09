#pragma once

#include "resource.hpp"

namespace iso {
    class Copper : public Resource { 
        private:
        public: 
            Copper();
            ~Copper();

            bool isGenerationSpotValid(GameObject* object, int index) const override;
            void placeResource(GameObject* object, int index) const override;

    };
}