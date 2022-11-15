#include "resource.hpp"
#include "region.hpp"
#include "simulationManager.hpp"
#include "globalutilities.hpp"
#include "item.hpp"

#include <iostream>
#include <lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace iso {
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

const core::Vector2i Resource::getResourceTextureSize() const {
    return this->data.texture_size;
}

const std::string& Resource::getIconTexture() const {
    return this->data.icon_texture;
}

const core::Vector2i Resource::getIconTextureSize() const {
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

bool Resource::isTileValid(int region_index, int tile_index) const {
    lua::runLuaFile(this->getDefinitionFilename());
    auto resource = luabridge::getGlobal(game_manager.lua(), "Resource");
    auto is_tile_valid = resource["isTileValid"];
    return is_tile_valid(region_index, tile_index);
}

bool Resource::isRegionValid(int region_index) const {
    lua::runLuaFile(this->getDefinitionFilename());
    auto resource = luabridge::getGlobal(game_manager.lua(), "Resource");
    auto is_region_valid = resource["isRegionValid"];
    return is_region_valid(region_index);
}

void Resource::placeResource(int region_index, int tile_index) const {
    auto& region = game_manager.world_map.at(region_index);
    region.map[tile_index].setTextureName(this->getResourceTexture());
    region.resources[tile_index] = *this;
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

void Resource::setResourceName(const std::string& name) {
    this->data.name = name;
}

void Resource::setResourceDescription(const std::string& description) {
    this->data.description = description;
}

void Resource::setResourceTexture(const std::string& texture) {
    this->data.texture = texture;
}

void Resource::setResourceIcon(const std::string& icon_texture) {
    this->data.icon_texture = icon_texture;
}

void Resource::setResourceType(const std::string& resource_type) {
    this->data.type = resource_type;
}

StorageItem Resource::asItem() const {
    for(const auto& item : ITEM_TABLE) {
        if(this->getResourceName() == item.getItemName())
            return item;
    }

    printError("Resource::asItem()", "Resource '" + this->getResourceName() + "' does not have a corresponding item");
    return StorageItem();
}
}