#include "tileType.hpp"
#include <iostream>

using namespace iso;

TileType::TileType() {
    this->type = std::map <std::string, bool> ({
        { "TERRAIN", false },
        { "RIVER"  , false },
        { "OCEAN"  , false },
        { "COAST"  , false },
    });
}

TileType::~TileType() {
    
}

bool TileType::is_type(const std::string& id) const {
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
    return this->is_type("TERRAIN");
}

bool TileType::is_water() const {
    return this->is_river() || this->is_ocean();
}

bool TileType::is_river() const {
    return this->is_type("RIVER");
}

bool TileType::is_ocean() const {
    return this->is_type("OCEAN");
}

bool TileType::is_coast() const {
    return this->is_type("COAST");
}

void TileType::set_terrain() {
    this->set_type("TERRAIN", true);
    this->set_type("OCEAN", false);
    this->set_type("RIVER", false);
}

void TileType::set_river() {
    this->set_type("RIVER", true);
    this->set_type("TERRAIN", false);
}

void TileType::set_ocean() {
    this->set_type("OCEAN", true);
    this->set_type("TERRAIN", false);
}

void TileType::set_coast() {
    this->set_type("COAST", true);
}