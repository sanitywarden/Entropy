#pragma once

#include "resource.hpp"

namespace iso {
    class Clay : public Resource {
        private:
        public:
            Clay();
            ~Clay();

            bool isGenerationSpotValid(GameObject* object, int index) const override;
            void placeResource(GameObject* object, int index) const override;
            bool canBeGenerated(GameObject* object) const override;
    };
}