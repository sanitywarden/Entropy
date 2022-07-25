#include "unit.hpp"

#include <iostream>

using namespace iso;

int Unit::id_count = 0;
Unit::Unit() 
    : GameObject(sf::Vector3f(0, 0, 0), sf::Vector3f(0, 0, 0), sf::Vector2f(0, 0), "*", "Unit")
{
    this->unit_name = "*";
    this->id = this->id_count;
    this->id_count++;
    
    this->current_index = 0;
    this->goal = 0;
    this->type = UnitType::UNITTYPES_TOTAL_QUANTITY;

    this->path.resize(0);
}

Unit::Unit(std::string unit_name)
    : GameObject(sf::Vector3f(0, 0, 0), sf::Vector3f(0, 0, 0), sf::Vector2f(0, 0), "*", "Unit")
{
    this->unit_name = unit_name;
    this->id = this->id_count;
    this->id_count++;
    
    this->current_index = 0;
    this->goal = 0;
    this->type = UnitType::UNITTYPES_TOTAL_QUANTITY;

    this->path.resize(0);
}

Unit::~Unit() 
{}

const std::vector <int>& Unit::getAStarPath() {
    return this->path;
}

bool Unit::hasPath() const {
    return !this->path.empty();
}

/* Returns next move of the pawn and deletes it from the list of moves. 
 * Does not set it as current index. */
int Unit::getNextMove() {
    if(!this->path.size())
        return -1;

    int next_move = this->path.front();
    this->path.erase(this->path.begin());
    return next_move;
}

void Unit::setNewPath(std::vector <int> path) {
    this->path = path;
}

int Unit::getID() const {
    return this->id;
}

bool Unit::operator== (const Unit& unit) const {
    return this->id == unit.getID();
}

bool Unit::operator!= (const Unit& unit) const {
    return !(*this == unit);
}

bool Unit::isSameType(UnitType type) const {
    return this->type == type;
}

std::string Unit::getUnitName() const {
    return this->unit_name;
}