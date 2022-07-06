#pragma once

#include "building.hpp"
#include "resource.hpp"

namespace iso {
    class Animal : public Building, public Resource {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        public:
            Animal();
            ~Animal();
    
            bool isGenerationSpotValid(GameObject* object, int index) const;
            void placeResource(GameObject* object, int index) const;
            bool isSingleObject() const override;
    };
}