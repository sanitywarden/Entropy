#include "resource.hpp"

using namespace iso;

Resource::Resource() 
{}

Resource::Resource(const Resource& resource)
    : data(resource.data)
{}

Resource::Resource(const ResourceData& data)
    : data(data)
{}

Resource::~Resource() 
{}

const std::string& Resource::getDefinitionFilename() const {
    return this->data.filename;
}

const std::string& Resource::getResourceName() const {
    return this->data.name;
}

const std::string& Resource::getResourceDescription() const {
    return this->data.description;
}

const std::string& Resource::getResourceTexture() const {
    return this->data.texture;
}

const sf::Vector2i Resource::getResourceTextureSize() const {
    return this->data.texture_size;
}

const std::string& Resource::getIconTexture() const {
    return this->data.icon_texture;
}

const sf::Vector2i Resource::getIconTextureSize() const {
    return this->data.icon_size;
}

const std::string& Resource::getResourceType() const {
    return this->data.type;
}

int Resource::getMinimumOccurence() const {
    return this->data.min_occurence;
}

int Resource::getMaximumOccurence() const {
    return this->data.max_occurence;
}
 
float Resource::getGenerationChance() const {
    return this->data.chance;
}

int Resource::getPatchSize() const {
    return this->data.patch_size;
}

bool Resource::isTileValid(GameObject* object) const {
    return false;
}

bool Resource::isRegionValid(GameObject* object) const {
    return false;
}

void Resource::placeResource(GameObject* object_r, GameObject* object_t, int index) const {

}

bool Resource::isSingleObject() const {
    return false;
}

bool Resource::operator== (const Resource& resource) const {
    return this->getResourceName() == resource.getResourceName(); 
}

bool Resource::operator!= (const Resource& resource) const {
    return !(*this == resource);
}
