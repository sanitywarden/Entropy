#include "building_definitions.hpp"

std::shared_ptr <iso::Building> iso::getBuildingPointer(const std::string& building_name) {
    for(const auto& ptr : BUILDING_LOOKUP_TABLE) {
        if(ptr.get()->getBuildingName() == building_name)
            return ptr;
    }

    return nullptr;
}