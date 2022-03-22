#include "resourceManager.hpp"

#include <iostream>
#include <ios>

using namespace entropy;

resourceManager::resourceManager() {
    sf::err().clear(std::ios::failbit);
}

resourceManager::resourceManager(const Settings& settings) {
    sf::err().clear(std::ios::failbit);

    this->m_settings = settings;
}

resourceManager::~resourceManager() {
    sf::err().clear();
}

void resourceManager::loadTexture(const std::string filename, const std::string id, const sf::IntRect area) {
    sf::Texture texture;
    if(!texture.loadFromFile(filename, area)) {
        std::cout << "[Entropy Engine][ResourceType Manager]: Could not load texture: " << filename << " with id: " << id << ".\n";
        return;
    }

    this->m_dimensions[id] = area;
    this->m_textures[id]   = texture;
}

void resourceManager::addTexture(const std::string& id, sf::Texture texture, const sf::IntRect area) {
    this->m_dimensions[id] = area;
    this->m_textures[id]   = texture;
}

void resourceManager::unloadTexture(const std::string id) {
    if(this->checkTextureExists(id)) {
        this->m_textures.erase(id);
        this->m_dimensions.erase(id);
        return;
    }

    std::cout << "[Entropy Engine][ResourceType Manager]: Could not unload texture: " << id << ".\n";
}

bool resourceManager::checkTextureExists(const std::string id) {
    return this->m_textures.count(id);
}

sf::Texture& resourceManager::getTexture(const std::string id) {
    return this->m_textures[id];
}

// If a texture was loaded from a bigger texture it can be used for drawing with Vertex Arrays.
// Manually writing a table with positions of a texture would be boring and frustrating,
// so you can reuse the infromation provided when loading the texture. 
sf::Vector2f resourceManager::getTexturePosition(const std::string& id) {
    sf::Vector2f position = sf::Vector2f(
        this->m_dimensions.at(id).left,
        this->m_dimensions.at(id).top
    );

    return position;
} 

sf::Vector2f resourceManager::getTextureSize(const std::string& id) {
    sf::Vector2f position = sf::Vector2f(
        this->m_dimensions.at(id).width,
        this->m_dimensions.at(id).height
    );
    return position;
}

sf::IntRect resourceManager::getTextureIntRect(const std::string& id) {
    if(this->m_dimensions.count(id))
        return this->m_dimensions.at(id);
    return sf::IntRect(0, 0, 0, 0);
}

const std::map <std::string, sf::Texture>& resourceManager::getTextureCollection() {
    return this->m_textures; 
}

void resourceManager::loadFont(const std::string filename, const std::string id) {
    sf::Font font;
    if(!font.loadFromFile(filename)) {
        std::cout << "[Entropy Engine][ResourceType Manager]: Could not load font: " << filename << " with id: " << id << ".\n";
        return;
    }

    this->m_fonts[id] = font;
}

void resourceManager::unloadFont(const std::string id) {
    if(this->checkFontExists(id)) {
        this->m_fonts.erase(id);
        return;
    }

    std::cout << "[Entropy Engine][ResourceType Manager]: Could not unload font: " << id << ".\n";
}

bool resourceManager::checkFontExists(const std::string id) {
    return this->m_fonts.count(id);
}

sf::Font& resourceManager::getFont(const std::string id) {
    return this->m_fonts[id];
}

const std::map <std::string, sf::Font>& resourceManager::getFontCollection() {
    return this->m_fonts;
}