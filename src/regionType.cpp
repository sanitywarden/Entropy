#include "regionType.hpp"
#include <iostream>

using namespace iso;

RegionType::RegionType() {
    this->type = std::map <std::string, bool> ({
        { "TERRAIN", false },
        { "COAST"  , false },
        { "RIVER"  , false },
        { "LAKE"   , false },
        { "FOREST" , false },
        { "OCEAN"  , false },
    });
}

RegionType::~RegionType() 
{}

bool RegionType::is_type(std::string id) const {
    try {
        return this->type.at(id);
    }
    catch(const std::exception& exception) {
        std::cout << "[RegionType]: Can not access ID " << id << "\n";
        return false;
    }
}

void RegionType::set_type(std::string id, bool value) {
    this->type[id] = value;
}

bool RegionType::is_terrain() const {
    return this->is_type("TERRAIN");
}

bool RegionType::is_ocean() const {
    return !this->is_type("TERRAIN");
}

bool RegionType::is_coast() const {
    return this->is_type("COAST");
}

bool RegionType::is_river() const {
    return this->is_type("RIVER");
}

bool RegionType::is_forest() const {
    return this->is_type("FOREST");
}

bool RegionType::is_lake() const {
    return this->is_type("LAKE");
}

bool RegionType::is_water() const {
    return this->is_river() || this->is_ocean() || this->is_river() && this->is_lake();
}

void RegionType::set_terrain() {
    this->set_type("TERRAIN", true);
    this->set_type("OCEAN", false);
}

void RegionType::set_ocean() {
    this->set_type("OCEAN", true);
    this->set_type("TERRAIN", false);
}

void RegionType::set_coast() {
    this->set_type("COAST", true);
}

void RegionType::set_river() {
    this->set_type("RIVER", true);
}

void RegionType::set_forest() {
    this->set_type("FOREST", true);
}

void RegionType::set_lake() {
    this->set_type("LAKE", true);
}

void RegionType::unset_coast() {
    this->set_type("COAST", false);
}