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
        std::cout << "[Entropy Engine][Resource Manager]: Could not load texture: " << filename << " with id: " << id << ".\n";
        return;
    }

    this->m_textures[id] = texture;
}

void resourceManager::unloadTexture(const std::string id) {
    if(this->checkTextureExists(id)) {
        this->m_textures.erase(id);
        return;
    }

    std::cout << "[Entropy Engine][Resource Manager]: Could not unload texture: " << id << ".\n";
}

bool resourceManager::checkTextureExists(const std::string id) {
    return this->m_textures.count(id);
}

sf::Texture& resourceManager::getTexture(const std::string id) {
    return this->m_textures[id];
}

const std::map <std::string, sf::Texture>& resourceManager::getTextureCollection() {
    return this->m_textures; 
}

void resourceManager::loadFont(const std::string filename, const std::string id) {
    sf::Font font;
    if(!font.loadFromFile(filename)) {
        std::cout << "[Entropy Engine][Resource Manager]: Could not load font: " << filename << " with id: " << id << ".\n";
        return;
    }

    this->m_fonts[id] = font;
}

void resourceManager::unloadFont(const std::string id) {
    if(this->checkFontExists(id)) {
        this->m_fonts.erase(id);
        return;
    }

    std::cout << "[Entropy Engine][Resource Manager]: Could not unload font: " << id << ".\n";
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