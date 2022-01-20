#include "regionType.hpp"

using namespace iso;

bitmask& RegionType::regionType() {
    return this->m_type;
}

bool RegionType::is_type(char bit) const {
    return this->m_type.test(bit);
}

void RegionType::set_type(char bit, bool value) {
    this->m_type.set(bit, value);
}

bool RegionType::is_terrain() const {
    return this->is_type(0);
}

bool RegionType::is_ocean() const {
    return !this->is_type(0);
}

bool RegionType::is_coast() const {
    return this->is_type(1);
}

bool RegionType::is_river() const {
    return this->is_type(2);
}

bool RegionType::is_forest() const {
    return this->is_type(3);
}

void RegionType::set_terrain() {
    this->set_type(0, true);
}

void RegionType::set_ocean() {
    this->set_type(0, false);
}

void RegionType::set_coast() {
    this->set_type(1, true);
}

void RegionType::set_river() {
    this->set_type(2, true);
}

void RegionType::set_forest() {
    this->set_type(3, true);
}