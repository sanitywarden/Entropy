#ifndef _TEXTURIZER_HPP_
#define _TEXTURIZER_HPP_

#include "./entropy/resourceManager.hpp"
#include "colours.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>

namespace iso {
    class Texturizer {
        private:
            resourceManager* resource;
        public:
            Texturizer();
            Texturizer(resourceManager* resource_manager);
            ~Texturizer();

            std::string createColouredWorldmapTexture(const std::string& id, const std::string& save_as, const sf::Color& replacement_black, const sf::Color& replacement_white);      
            std::string blendTextures(const std::string& save_as, const std::string& texture_1, const std::string& texture_2);
            sf::Color   getRandomColour() const;
    };
}

#endif