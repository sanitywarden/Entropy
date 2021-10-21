#include "region.hpp"

using namespace iso;

Region::Region() {
    this->_marked    = false;
    this->_direction = RiverDirection::RIVER_NONE;

    this->height      = 0.0f;
    this->latitude    = 0.0f;
    this->moisture    = 0.0f;
    this->temperature = 0.0f;

    this->visited = false;
}

Region::~Region() {

}

// Remember that you can implement fog of war by colouring the regions texture (sf::Color).

void Region::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray worldmap_region(sf::Quads, 4);

    worldmap_region[0].position = this->position;
    worldmap_region[1].position = this->position + sf::Vector2f(this->size.x, 0);
    worldmap_region[2].position = this->position + sf::Vector2f(this->size.x, this->size.y);
    worldmap_region[3].position = this->position + sf::Vector2f(0, this->size.y);
    
    worldmap_region[0].texCoords = sf::Vector2f(0, 0);
    worldmap_region[1].texCoords = sf::Vector2f(this->size.x, 0);
    worldmap_region[2].texCoords = sf::Vector2f(this->size.x, this->size.y);
    worldmap_region[3].texCoords = sf::Vector2f(0, this->size.y);

    states.texture = &this->texture;
    target.draw(worldmap_region, states);
    
    // Draw region's features to the worldmap.
    if(this->river.exists()) 
        target.draw(this->river);

    if(this->forest.exists())
        target.draw(this->forest);
}

RiverDirection Region::riverDirection() {
    return this->_direction;
}