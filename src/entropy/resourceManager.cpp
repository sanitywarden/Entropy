#include "resourceManager.hpp"

#include <iostream>
#include <ios>

using namespace iso;

ResourceManager::ResourceManager() 
{}

ResourceManager::ResourceManager(const Settings& settings) 
    : m_settings(settings)
{}

ResourceManager::~ResourceManager() 
{}

void ResourceManager::loadTexture(const std::string filename, const std::string id, sf::IntRect area) {
    sf::Texture texture;
    if(!texture.loadFromFile(filename, area)) {
        std::cout << "[Entropy Engine][Resource Manager]: Could not load texture: " << filename << " with id: " << id << ".\n";
        return;
    }

    this->m_dimensions[id] = area;
    this->m_textures[id]   = texture;
}

void ResourceManager::addTexture(const std::string& id, sf::Texture texture, const sf::IntRect area) {
    this->m_dimensions[id] = area;
    this->m_textures[id]   = texture;
}

void ResourceManager::unloadTexture(const std::string id) {
    if(this->checkTextureExists(id)) {
        this->m_textures.erase(id);
        this->m_dimensions.erase(id);
        return;
    }

    std::cout << "[Entropy Engine][Resource Manager]: Could not unload texture: " << id << ".\n";
}

bool ResourceManager::checkTextureExists(const std::string id) const {
    return this->m_textures.count(id);
}

sf::Texture& ResourceManager::getTexture(const std::string id) {
    return this->m_textures[id];
}

// If a texture was loaded from a bigger texture it can be used for drawing with Vertex Arrays.
// Manually writing a table with positions of a texture would be boring and frustrating,
// so you can reuse the infromation provided when loading the texture. 
sf::Vector2f ResourceManager::getTexturePosition(const std::string& id) const {
    try {
        sf::Vector2f position = sf::Vector2f(
            this->m_dimensions.at(id).left,
            this->m_dimensions.at(id).top
        );

        return position;
    }
    catch(const std::out_of_range& exception) {
        std::cout << "[Resource Manager]: getTexturePosition(): Texture with ID '" << id << "' does not exist.\n";
        throw exception;
    }

    return sf::Vector2f(0, 0);
} 

sf::Vector2f ResourceManager::getTextureSize(const std::string& id) const {
    try {
        sf::Vector2f size = sf::Vector2f(
            this->m_dimensions.at(id).width,
            this->m_dimensions.at(id).height
        );
        return size;
    }

    catch(const std::out_of_range& exception) {
        std::cout << "[Resource Manager]: getTextureSize(): Texture with ID '" << id << "' does not exist.\n";
        throw exception;
    }

    return sf::Vector2f(0, 0);
}

sf::IntRect ResourceManager::getTextureIntRect(const std::string& id) const {
    if(this->m_dimensions.count(id))
        return this->m_dimensions.at(id);
    return sf::IntRect(0, 0, 0, 0);
}

const std::map <std::string, sf::Texture>& ResourceManager::getTextureCollection() const {
    return this->m_textures; 
}

void ResourceManager::loadFont(const std::string filename, const std::string id) {
    sf::Font font;
    if(!font.loadFromFile(filename)) {
        std::cout << "[Entropy Engine][Resource Manager]: Could not load font: " << filename << " with id: " << id << ".\n";
        return;
    }

    this->m_fonts[id] = font;
}

void ResourceManager::unloadFont(const std::string id) {
    if(this->checkFontExists(id)) {
        this->m_fonts.erase(id);
        return;
    }

    std::cout << "[Entropy Engine][Resource Manager]: Could not unload font: " << id << ".\n";
}

bool ResourceManager::checkFontExists(const std::string id) {
    return this->m_fonts.count(id);
}

sf::Font& ResourceManager::getFont(const std::string id) {
    return this->m_fonts[id];
}

const std::map <std::string, sf::Font>& ResourceManager::getFontCollection() {
    return this->m_fonts;
}