#include "unit.hpp"

#include <iostream>

using namespace iso;

int Unit::id_count = 0;
Unit::Unit() {
    this->object_name = "unit";
    this->id = this->id_count;
    this->id_count++;
    
    this->current_index = 0;

    this->path.resize(0);
}

Unit::~Unit() {

}

const std::vector <int>& Unit::getAStarPath() {
    return this->path;
}

bool Unit::hasPath() const {
    return !this->path.empty();
}

/* Returns next move of the pawn and deletes it from the list of moves. */
int Unit::getNextMove() {
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

void Unit::setNewPath(std::vector <int> path) {
    this->path = path;
}

int Unit::getID() const {
    return this->id;
}