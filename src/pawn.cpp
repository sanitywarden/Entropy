#include "pawn.hpp"

#include <iostream>

using namespace iso;

Pawn::Pawn() {

}

Pawn::Pawn(Region* region) {
    this->region        = region;
    this->object_name   = "pawn";
    this->current_index = 0;
    this->path.resize(0);

    this->object_size         = sf::Vector2f(16, 16);
    this->object_texture_name = "actor";
    this->object_position     = this->region->map[this->current_index].getPosition();
}

Pawn::~Pawn() {

}

const std::vector <int>& Pawn::getAStarPath() {
    return this->path;
}

bool Pawn::hasPath() {
    return !this->path.empty();
}

void Pawn::nextMove() {
    if(!this->path.size())
        return;

    this->current_index = this->path.front();
    const Tile& tile    = this->region->map[this->current_index];
    this->object_position = tile.getPosition();
    
    for(auto iterator = this->path.begin(); iterator != this->path.end(); ++iterator) {
        if(*iterator == this->current_index) {
            this->path.erase(iterator);
            return;
        }
    }
}
