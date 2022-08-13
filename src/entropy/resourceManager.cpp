#include "resourceManager.hpp"

#include <iostream>
#include <ios>

using namespace iso;

ResourceManager::ResourceManager() 
{}

ResourceManager::~ResourceManager() 
{}

void ResourceManager::loadTexture(const std::string& filename, const std::string& id, sf::IntRect area) {
    sf::Texture texture;
    if(!texture.loadFromFile(filename, area)) {
        std::cout << "[Entropy Engine][Resource Manager]: Could not load texture: " << filename << " with id: " << id << ".\n";
        return;
    }

    this->dimensions[id] = area;
    this->textures[id]   = texture;
}

void ResourceManager::addTexture(const std::string& id, sf::Texture texture, const sf::IntRect area) {
    this->dimensions[id] = area;
    this->textures[id]   = texture;
}

void ResourceManager::unloadTexture(const std::string& id) {
    if(this->checkTextureExists(id)) {
        this->textures.erase(id);
        this->dimensions.erase(id);
        return;
    }

    std::cout << "[Entropy Engine][Resource Manager]: Could not unload texture: " << id << ".\n";
}

bool ResourceManager::checkTextureExists(const std::string& id) const {
    return this->textures.count(id);
}

sf::Texture& ResourceManager::getTexture(const std::string& id) {
    return this->textures[id];
}

// If a texture was loaded from a bigger texture it can be used for drawing with Vertex Arrays.
// Manually writing a table with positions of a texture would be boring and frustrating,
// so you can reuse the infromation provided when loading the texture. 
sf::Vector2f ResourceManager::getTexturePosition(const std::string& id) const {
    try {
        sf::Vector2f position = sf::Vector2f(
            this->dimensions.at(id).left,
            this->dimensions.at(id).top
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
            this->dimensions.at(id).width,
            this->dimensions.at(id).height
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
    if(this->dimensions.count(id))
        return this->dimensions.at(id);
    return sf::IntRect(0, 0, 0, 0);
}

const std::map <std::string, sf::Texture>& ResourceManager::getTextureCollection() const {
    return this->textures; 
}

void ResourceManager::loadFont(const std::string& filename, const std::string& id) {
    sf::Font font;
    if(!font.loadFromFile(filename)) {
        std::cout << "[Entropy Engine][Resource Manager]: Could not load font: " << filename << " with id: " << id << ".\n";
        return;
    }

    this->fonts[id] = font;
}

void ResourceManager::unloadFont(const std::string& id) {
    if(this->checkFontExists(id)) {
        this->fonts.erase(id);
        return;
    }

    std::cout << "[Entropy Engine][Resource Manager]: Could not unload font: " << id << ".\n";
}

bool ResourceManager::checkFontExists(const std::string& id) {
    return this->fonts.count(id);
}

sf::Font& ResourceManager::getFont(const std::string& id) {
    return this->fonts[id];
}

const std::map <std::string, sf::Font>& ResourceManager::getFontCollection() {
    return this->fonts;
}