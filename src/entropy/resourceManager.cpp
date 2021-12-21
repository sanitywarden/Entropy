#include "resourceManager.hpp"

#include <iostream>

using namespace entropy;

static std::ios_base::iostate m_sfmlerror;

resourceManager::resourceManager() {
    sf::err().clear(std::ios::failbit);
}

resourceManager::~resourceManager() {
    sf::err().clear(m_sfmlerror);
}

void resourceManager::loadTexture(std::string filename, std::string id, sf::IntRect area) {
    sf::Texture texture;
    if(!texture.loadFromFile(filename, area)) {
        std::cout << "[Entropy Engine][Resource Manager]: Could not load texture: " << filename << " with id: " << id << ".\n";
        return;
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
        std::cout << "[Entropy Engine][Resource Manager]: Could not load font: " << filename << " with id: " << id << ".\n";
        return;
    }

    this->m_fonts[id] = font;
}

sf::Font& resourceManager::getFont(std::string id) {
    return this->m_fonts[id];
}

const std::map <std::string, sf::Font>& resourceManager::getFontCollection() {
    return this->m_fonts;
}