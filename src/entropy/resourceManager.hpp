#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Err.hpp>
#include <map>

namespace iso {
class ResourceManager {
    private:
        std::map <std::string, sf::Texture> textures;
        std::map <std::string, sf::IntRect> dimensions;
        std::map <std::string, sf::Font>    fonts;
    public:
        ResourceManager();
        ~ResourceManager();

        void loadTexture(const std::string& filename, const std::string& id, sf::IntRect area = sf::IntRect());
        void addTexture(const std::string& id, sf::Texture texture, const sf::IntRect area = sf::IntRect());
        void unloadTexture(const std::string& id);
        bool checkTextureExists(const std::string& id) const;
        sf::Texture& getTexture(const std::string& id);
        sf::Vector2f getTexturePosition(const std::string& id) const;
        sf::Vector2f getTextureSize(const std::string& id)     const;
        sf::IntRect  getTextureIntRect(const std::string& id)  const;
        const std::map <std::string, sf::Texture>& getTextureCollection() const;

        void loadFont(const std::string& filename, const std::string& id);
        void unloadFont(const std::string& id);
        bool checkFontExists(const std::string& id);
        sf::Font& getFont(const std::string& id);
        const std::map <std::string, sf::Font>& getFontCollection();
};
}