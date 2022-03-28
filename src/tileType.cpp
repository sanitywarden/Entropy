#include "tileType.hpp"

using namespace iso;

bitmask& TileType::tileType() {
    return this->m_type;
}

bool TileType::is_type(char bit) const {
    return this->m_type.test(bit);
}

void TileType::set_type(char bit, bool value) {
    this->m_type.set(bit, value);
}

bool TileType::is_terrain() const {
    return this->is_type(0);
}

bool TileType::is_water() const {
    return (this->is_ocean() || this->is_river());
}

bool TileType::is_river() const {
    return this->is_type(1);
}

bool TileType::is_ocean() const {
    return !this->is_type(1);
}

void TileType::set_terrain() {
    this->set_type(0, true);
}

void TileType::set_water() {
    this->set_type(0, false);
}

void TileType::set_river() {
    this->set_type(1, true);
}

void TileType::set_ocean() {
    this->set_type(1, false);
}