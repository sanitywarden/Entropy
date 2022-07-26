#include "resource.hpp"

using namespace iso;

Resource::Resource()
    : resource_name("None"), resource_icon("icon_default"), resource_type(ResourceType::TYPE_UNCATEGORISED), min_occurence(0), max_occurence(0) 
{}

Resource::Resource(const std::string& name, ResourceType type)
    : resource_name(name), resource_icon("icon_default"), resource_type(type), min_occurence(0), max_occurence(0) 
{}

Resource::Resource(const std::string& name, ResourceType type, const std::string& icon)
    : resource_name(name), resource_icon(icon), resource_type(type), min_occurence(0), max_occurence(0) 
{}

Resource::Resource(const std::string& name, ResourceType type, const std::string& icon, int min, int max)
    : resource_name(name), resource_icon(icon), resource_type(type), min_occurence(min), max_occurence(max)
{}

Resource::~Resource() 
{}

bool Resource::operator== (const Resource& resource) const {
    return this->resource_name == resource.resource_name; 
}

bool Resource::operator!= (const Resource& resource) const {
    return !(*this == resource);
}

const std::string& Resource::getResourceName() const {
    return this->resource_name;
}

ResourceType Resource::getResourceType() const {
    return this->resource_type;
}
