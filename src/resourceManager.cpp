#include "resourceManager.hpp"
#include "colours.hpp"
#include "globalutilities.hpp"

#include <iostream>
#include <ios>

namespace iso {
ResourceManager::ResourceManager() 
{}

ResourceManager::~ResourceManager() 
{}

void ResourceManager::loadTexture(const std::string& filename, const std::string& id, sf::IntRect area) {
    if(this->checkTextureExists(id))
        return;

    sf::Texture texture;
    if(!texture.loadFromFile(filename, area)) {
        printError("ResourceManager::loadTexture()", "Could not load texture '" + filename + "' with id '" + id + "'");
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
}

bool ResourceManager::checkTextureExists(const std::string& id) const {
    return this->textures.count(id);
}

sf::Texture& ResourceManager::getTexture(const std::string& id) {
    if(!this->checkTextureExists(id))
        printError("ResourceManager::getTexure()", "There is no texture with id '" + id + "'");
    return this->textures.at(id);
}

// If a texture was loaded from a bigger texture it can be used for drawing with Vertex Arrays.
// Manually writing a table with positions of a texture would be boring and frustrating,
// so you can reuse the infromation provided when loading the texture. 
core::Vector2f ResourceManager::getTexturePosition(const std::string& id) const {
    try {
        auto position = core::Vector2f(
            this->dimensions.at(id).left,
            this->dimensions.at(id).top
        );

        return position;
    }
    catch(const std::out_of_range& exception) {
        printError("ResourceManager::getTexturePosition()", "Texture with id '" + id + "' does not exist.");
        return core::Vector2f(0, 0);
    }

    return core::Vector2f(0, 0);
} 

core::Vector2f ResourceManager::getTextureSize(const std::string& id) const {
    try {
        auto size = core::Vector2f(
            this->dimensions.at(id).width,
            this->dimensions.at(id).height
        );
        return size;
    }

    catch(const std::out_of_range& exception) {
        printError("ResourceManager::getTextureSize()", "Texture with id '" + id + "' does not exist.");
        return core::Vector2f(0, 0);
    }

    return core::Vector2f(0, 0);
}

sf::IntRect ResourceManager::getTextureIntRect(const std::string& id) const {
    try {
        auto dimensions = this->dimensions.at(id);
        return dimensions;
    }

    catch(const std::out_of_range& exception) {
        printError("ResourceManager::getTextureIntRect()", "Texture with id '" + id + "' does not exist.");
        return sf::IntRect(0, 0, 0, 0);
    }

    return sf::IntRect(0, 0, 0, 0);
}

const std::map <std::string, sf::Texture>& ResourceManager::getTextureCollection() const {
    return this->textures; 
}

void ResourceManager::loadFont(const std::string& filename, const std::string& id) {
    sf::Font font;
    if(!font.loadFromFile(filename)) {
        printError("ResourceManager::loadFont()", "Could not load font file '" + filename + "' with id '" + id + "'");
        return;
    }

    this->fonts[id] = font;
}

void ResourceManager::unloadFont(const std::string& id) {
    if(this->checkFontExists(id)) {
        this->fonts.erase(id);
        return;
    }
}

bool ResourceManager::checkFontExists(const std::string& id) {
    return this->fonts.count(id);
}

sf::Font& ResourceManager::getFont(const std::string& id) {
    if(!this->checkFontExists(id))
        printError("ResourceManager::getFont()", "There is no font with id '" + id + "'");
    return this->fonts.at(id);
}

const std::map <std::string, sf::Font>& ResourceManager::getFontCollection() {
    return this->fonts;
}

void ResourceManager::createPanelBiomeVariant(const std::string& base_panel_texture, const Biome& biome) {
    std::string texture_id = base_panel_texture + "_" + biome.getBiomeId();

    if(this->checkTextureExists(texture_id))
        return;
    
    auto image   = this->getTexture(base_panel_texture).copyToImage();
    auto intrect = this->getTextureIntRect(base_panel_texture);

    for(int y = 0; y < image.getSize().y; y++) {
        for(int x = 0; x < image.getSize().x; x++) {
            // Fill in terrain pixel.
            if(image.getPixel(x, y) == COLOUR_BLACK.asSFMLColour())
                image.setPixel(x, y, biome.getBiomeWorldmapColour().asSFMLColour());
            
            // Fill in water pixel.
            else if(image.getPixel(x, y) == COLOUR_WHITE.asSFMLColour())
                image.setPixel(x, y, COLOUR_BLUE_OCEAN.asSFMLColour());
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    this->addTexture(texture_id, texture, intrect);
}

void ResourceManager::paintTexture(const std::string& id, const std::string& save_as, core::Colour replacement_black, core::Colour replacement_white) {
    if(this->checkTextureExists(save_as))
        return;

    auto image   = this->getTexture(id).copyToImage();
    auto intrect = this->getTextureIntRect(id);

    for(int y = 0; y < image.getSize().y; y++) {
        for(int x = 0; x < image.getSize().x; x++) {
            if(image.getPixel(x, y) == COLOUR_BLACK.asSFMLColour())
                image.setPixel(x, y, replacement_black.asSFMLColour());
            
            else if(image.getPixel(x, y) == COLOUR_WHITE.asSFMLColour())
                image.setPixel(x, y, replacement_white.asSFMLColour());
        }
    } 

    sf::Texture texture;
    texture.loadFromImage(image);
    this->addTexture(save_as, texture, intrect);
}

void ResourceManager::blendTextures(const std::string& save_as, const std::string& texture_1, const std::string& texture_2) {
    if(this->checkTextureExists(save_as))
        return;

    auto image_1 = this->getTexture(texture_1).copyToImage();
    auto image_2 = this->getTexture(texture_2).copyToImage();

    for(int y = 0; y < image_1.getSize().y; y++) {
        for(int x = 0; x < image_1.getSize().x; x++) {
            auto colour = image_2.getPixel(x, y);
            if(colour.a != 0) {
                auto colour = image_2.getPixel(x, y);
                image_1.setPixel(x, y, colour);
            }
        }
    }
    
    sf::Texture texture;
    auto intrect = this->getTextureIntRect(texture_1);
    texture.loadFromImage(image_1);
    this->addTexture(save_as, texture, intrect);
}
}