#include "regionType.hpp"
#include <iostream>

using namespace iso;

RegionType::RegionType() {
    this->type = std::map <std::string, bool> ({
        { "terrain", false },
        { "coast"  , false },
        { "river"  , false },
        { "forest" , false }
    });
}

RegionType::~RegionType() {

}

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
    return this->is_type("terrain");
}

bool RegionType::is_ocean() const {
    return !this->is_type("terrain");
}

bool RegionType::is_coast() const {
    return this->is_type("coast");
}

bool RegionType::is_river() const {
    return this->is_type("river");
}

bool RegionType::is_forest() const {
    return this->is_type("forest");
}

void RegionType::set_terrain() {
    this->set_type("terrain", true);
}

void RegionType::set_ocean() {
    this->set_type("terrain", false);
}

void RegionType::set_coast() {
    this->set_type("coast", true);
}

void RegionType::set_river() {
    this->set_type("river", true);
}

void RegionType::set_forest() {
    this->set_type("forest", true);
}

void RegionType::unset_coast() {
    this->set_type("coast", false);
}