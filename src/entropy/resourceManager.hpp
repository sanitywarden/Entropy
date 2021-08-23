#ifndef _ENTROPY_RESOURCE_MANAGER_HPP_
#define _ENTROPY_RESOURCE_MANAGER_HPP_

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include <map>

namespace entropy {
    // This class works as a easy to use resource manager.
    // It is used by Entropy, and there is only one instance of it per instance of the engine.
    // This allows to manage when to load or unload resources that the application uses. 
    class resourceManager {
        private:
            std::map <std::string, sf::Texture> m_textures;
            std::map <std::string, sf::Font>    m_fonts;

        public:
            resourceManager();
            ~resourceManager();

            void loadTexture(std::string filename, std::string id, sf::IntRect area = sf::IntRect());
            sf::Texture& getTexture(std::string id);
            const std::map <std::string, sf::Texture>& getTextureCollection();

            void loadFont(std::string filename, std::string id);
            sf::Font& getFont(std::string id);
            const std::map <std::string, sf::Font>& getFontCollection();
    };
}

#endif