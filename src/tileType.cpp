#include "tileType.hpp"
#include <iostream>

using namespace iso;

TileType::TileType() {
    this->type = std::map <std::string, bool> ({
        { "terrain", false },
        { "river"  , false },
        { "ocean"  , false }
    });
}

TileType::~TileType() {
    
}

bool TileType::is_type(std::string id) const {
    try {
        return this->type.at(id);
    }
    catch(const std::exception& exception) {
        std::cout << "[TileType]: Can not access ID " << id << "\n";
        return false;
    }
}

void TileType::set_type(std::string id, bool value) {
    this->type[id] = value;
}

bool TileType::is_terrain() const {
    return this->is_type("terrain");
}

bool TileType::is_water() const {
    return this->is_river() || this->is_ocean();
}

bool TileType::is_river() const {
    return this->is_type("river");
}

bool TileType::is_ocean() const {
    return this->is_type("ocean");
}

void TileType::set_terrain() {
    this->set_type("terrain", true);
    this->set_type("ocean", false);
    this->set_type("river", false);
}

void TileType::set_river() {
    this->set_type("river", true);
    this->set_type("terrain", false);
}

void TileType::set_ocean() {
    this->set_type("ocean", true);
    this->set_type("terrain", false);
}