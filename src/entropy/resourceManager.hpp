#ifndef _ENTROPY_RESOURCE_MANAGER_HPP_
#define _ENTROPY_RESOURCE_MANAGER_HPP_

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Err.hpp>
#include <map>

#include "engineSettings.hpp"

namespace entropy {
    // This class works as a easy to use resource manager.
    // It is used by Entropy, and there is only one instance of it per instance of the engine.
    // This allows to manage when to load or unload resources that the application uses. 
    class resourceManager {
        private:
            Settings m_settings;
            
            std::map <std::string, sf::Texture> m_textures;
            std::map <std::string, sf::Font>    m_fonts;
        public:
            resourceManager();
            resourceManager(const Settings& settings);
            ~resourceManager();

            void loadTexture(const std::string filename, const std::string id, const sf::IntRect area = sf::IntRect());
            void unloadTexture(const std::string id);
            bool checkTextureExists(const std::string id);
            sf::Texture& getTexture(const std::string id);
            const std::map <std::string, sf::Texture>& getTextureCollection();

            void loadFont(const std::string filename, const std::string id);
            void unloadFont(const std::string id);
            bool checkFontExists(const std::string id);
            sf::Font& getFont(const std::string id);
            const std::map <std::string, sf::Font>& getFontCollection();
    };
}

#endif