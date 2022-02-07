#include "pawn.hpp"

#include <iostream>

using namespace iso;

Pawn::Pawn() {
    this->object_name   = "pawn";
    this->current_index = 0;
    this->path.resize(0);

    this->object_size         = sf::Vector2f(32, 32);
    this->object_texture_name = "actor";
}

Pawn::~Pawn() {

}

const std::vector <int>& Pawn::getAStarPath() {
    return this->path;
}

bool Pawn::hasPath() {
    return !this->path.empty();
}

/* Returns next move of the pawn and deletes it from the list of moves. */
int Pawn::getNextMove() {
    if(!this->path.size())
        return -1;

    this->current_index = this->path.front();
    
    for(auto iterator = this->path.begin(); iterator != this->path.end(); ++iterator) {
        if(*iterator == this->current_index) {
            this->path.erase(iterator);
            break;
        }
    }

    return this->current_index;
}

void Pawn::setNewPath(std::vector <int> path) {
    this->path = path;
}