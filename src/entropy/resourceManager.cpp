#include "resourceManager.hpp"

#include <iostream>

entropy::resourceManager::resourceManager() {}
entropy::resourceManager::~resourceManager() {}

void entropy::resourceManager::loadTexture(std::string filename, std::string id, sf::IntRect area) {
    sf::Texture texture;
    
    if(!texture.loadFromFile(filename, area)) {
        std::cerr << "resourceManager: Could not load texture: " << filename << " with id: " << id << ".\n";
    }

    this->m_textures[id] = texture;
}

sf::Texture& entropy::resourceManager::getTexture(std::string id) {
    return this->m_textures[id];
}

const std::map <std::string, sf::Texture>& entropy::resourceManager::getTextureCollection() {
    return this->m_textures; 
}

void entropy::resourceManager::loadFont(std::string filename, std::string id) {
    sf::Font font;

    if(!font.loadFromFile(filename)) {
        std::cerr << "resourceManager: Could not load font: " << filename << " with id: " << id << ".\n";
    }

    this->m_fonts[id] = font;
}

sf::Font& entropy::resourceManager::getFont(std::string id) {
    return this->m_fonts[id];
}

const std::map <std::string, sf::Font>& entropy::resourceManager::getFontCollection() {
    return this->m_fonts;
}