#include "resource_definitions.hpp"

std::shared_ptr <iso::Resource> iso::getResourcePointer(const std::string& resource_name) {
    for(const auto& ptr : RESOURCE_LOOKUP_TABLE) {
        if(ptr.get()->resource_name == resource_name)
            return ptr;
    }

    return nullptr;
} 