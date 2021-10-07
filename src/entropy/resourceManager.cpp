#include "resourceManager.hpp"

#include <iostream>

using namespace entropy;

resourceManager::resourceManager() {

}

resourceManager::~resourceManager() {

}

void resourceManager::loadTexture(std::string filename, std::string id, sf::IntRect area) {
    sf::Texture texture;
    
    if(!texture.loadFromFile(filename, area)) {
        std::cerr << "[Entropy Engine][Resource Manager]: Could not load texture: " << filename << " with id: " << id << ".\n";
    }

    this->m_textures[id] = texture;
}

sf::Texture& resourceManager::getTexture(std::string id) {
    return this->m_textures[id];
}

const std::map <std::string, sf::Texture>& resourceManager::getTextureCollection() {
    return this->m_textures; 
}

void resourceManager::loadFont(std::string filename, std::string id) {
    sf::Font font;

    if(!font.loadFromFile(filename)) {
        std::cerr << "[Entropy Engine][Resource Manager]: Could not load font: " << filename << " with id: " << id << ".\n";
    }

    this->m_fonts[id] = font;
}

sf::Font& resourceManager::getFont(std::string id) {
    return this->m_fonts[id];
}

const std::map <std::string, sf::Font>& resourceManager::getFontCollection() {
    return this->m_fonts;
}