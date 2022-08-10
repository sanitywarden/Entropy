#include "resource.hpp"
#include "region.hpp"

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

bool Resource::isTileValid(GameObject* object, int index) const {
    const auto* region = (Region*)(object);
    const auto& tile = region->map[index];

    int requirements_passed = 0;
    int requirements_all = this->data.tile_requirements.size();

    if(requirements_passed == requirements_all)
        return true;

    for(const auto& property : this->data.tile_requirements) {        
        if(property == "TERRAIN" && tile.tiletype.is_terrain())
            requirements_passed++;

        if(property == "WATER" && tile.tiletype.is_water())
            requirements_passed++;

        if(property == "UNOCCUPIED" && !region->isSpotOccupied(index))
            requirements_passed++;

        if(property == "RIVER" && tile.tiletype.is_river())
            requirements_passed++;
        
        if(property == "OCEAN" && tile.tiletype.is_ocean())
            requirements_passed++;
        
        if(property == "COAST" && tile.tiletype.is_coast())
            requirements_passed++;
    }

    return requirements_passed == requirements_all;
}

bool Resource::isRegionValid(GameObject* object) const {
    const auto* region = (Region*)(object);
    
    int requirements_passed = 0;
    int requirements_all = this->data.region_requirements.size();

    if(requirements_passed == requirements_all) 
        return true;

    for(const auto& property : this->data.region_requirements) {
        if(property == "ALL")
            return true;
        
        if(property == "WATER" && region->regiontype.is_water())
            requirements_passed++;
        
        if(property == "DRY" && !region->regiontype.is_water())
            requirements_passed++;
        
        if(property == "LAKE" && region->regiontype.is_lake())  
            requirements_passed++;
        
        if(property == "RIVER" && region->regiontype.is_river())
            requirements_passed++;

        if(property == "COAST" && region->regiontype.is_coast())
            requirements_passed++;

        if(property == "FOREST" && region->regiontype.is_forest())
            requirements_passed++;

        if(property == "COLD" && region->temperature_text == "COLD")
            requirements_passed++;
        
        if(property == "WARM" && region->temperature_text == "WARM")
            requirements_passed++;
        
        if(property == "TROPICAL" && region->temperature_text == "TROPICAL")
            requirements_passed++;
        
        if(property == "HOT" && region->temperature_text == "HOT")
            requirements_passed++;
    }

    return requirements_passed == requirements_all;
}

void Resource::placeResource(GameObject* object, int index) const {
    auto* region = (Region*)(object);
    region->map[index].object_texture_name = this->getResourceTexture();
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

bool Resource::canBeGenerated(GameObject* object) const {
    return true;
}