#include "./texturizer.hpp"
#include "globalutilities.hpp"

iso::Texturizer::Texturizer() 
    : resource(nullptr) {}

iso::Texturizer::Texturizer(entropy::resourceManager* resource_manager)
    : resource(resource_manager) {}

iso::Texturizer::~Texturizer() {

}

std::string iso::Texturizer::createColouredWorldmapTexture(const std::string& id, const std::string& save_as, const sf::Color& replacement_black, const sf::Color& replacement_white) {
    if(this->resource->checkTextureExists(save_as)) 
        return save_as;

    auto og_image = this->resource->getTexture(id).copyToImage();
    auto intrect  = this->resource->getTextureIntRect(id);

    for(int y = 0; y < og_image.getSize().y; y++) {
        for(int x = 0; x < og_image.getSize().x; x++) {
            if(og_image.getPixel(x, y) == COLOUR_BLACK)
                og_image.setPixel(x, y, replacement_black);
            
            else if(og_image.getPixel(x, y) == COLOUR_WHITE)
                og_image.setPixel(x, y, replacement_white);
        }
    }

    sf::Texture texture;
    texture.loadFromImage(og_image);
    this->resource->addTexture(save_as, texture, intrect);
    return save_as;
}

sf::Color iso::Texturizer::getRandomColour() const {
    const int x = rand() % 16;
    const int y = rand() % 16;

    const auto& texture = this->resource->getTexture("random_colour");
    auto image  = texture.copyToImage();
    auto colour = image.getPixel(x * 16, y * 16);
    return colour; 
}