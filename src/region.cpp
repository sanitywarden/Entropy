#include "region.hpp"

using namespace iso;

Region::Region() {
    this->_marked    = false;
    this->_direction = RiverDirection::RIVER_NONE;

    this->height      = 0.0f;
    this->latitude    = 0.0f;
    this->moisture    = 0.0f;
    this->temperature = 0.0f;

    this->colour  = sf::Color::Black;
    this->visited = false;
}

Region::~Region() {

}

// Remember that you can implement fog of war by colouring the regions texture (sf::Color).
void Region::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray worldmap_region(sf::Quads, 4);

    const sf::Vector2f position = this->getPosition();

    worldmap_region[0].position = position;
    worldmap_region[1].position = position + sf::Vector2f(this->getSize().x, 0);
    worldmap_region[2].position = position + sf::Vector2f(this->getSize().x, this->getSize().y);
    worldmap_region[3].position = position + sf::Vector2f(0, this->getSize().y);
    
    worldmap_region[0].texCoords = sf::Vector2f(0, 0);
    worldmap_region[1].texCoords = sf::Vector2f(this->getSize().x, 0);
    worldmap_region[2].texCoords = sf::Vector2f(this->getSize().x, this->getSize().y);
    worldmap_region[3].texCoords = sf::Vector2f(0, this->getSize().y);

    if(this->colour != COLOUR_BLACK && this->colour != COLOUR_WHITE) {
        worldmap_region[0].color = this->colour; 
        worldmap_region[1].color = this->colour;
        worldmap_region[2].color = this->colour;
        worldmap_region[3].color = this->colour;
    }

    target.draw(worldmap_region, states);
}

RiverDirection Region::riverDirection() {
    return this->_direction;
}