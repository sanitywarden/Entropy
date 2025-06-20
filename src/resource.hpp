#pragma once

#include "gameObject.hpp"
#include "types.hpp"

#include <vector>
#include <string>

namespace iso {
struct ResourceData {
    std::string    filename;      // Path to the definition file of the resource.
    std::string    name;          // Human readable name.
    std::string    description;   // Human readable description.
    std::string    texture;       // Resource texture.
    core::Vector2i texture_size;  // Texture size in pixels.
    std::string    icon_texture;  // Icon texture.
    core::Vector2i icon_size;     // Icon size in pixels.
    std::string    type;          // Resource type.
    int            min_occurence; // How many patches of this resource should be generated.
    int            max_occurence; // How many patches of this resource may be generated.
    float          chance;        // Generation chance (0.0 : 1.0).
    int            patch_size;    // Certain resources generate in patches.
};

class StorageItem;

// Resource is a special commodity that may be generated in a region upon visiting.
// They may be common or rare.
// Certain resources exist only in specific areas of the world. 
class Resource {
    protected:
        ResourceData data;
    
    public:
        Resource();
        Resource(const Resource& resource);
        Resource(const ResourceData& data);
        ~Resource();
        
        const std::string&   getDefinitionFilename()  const;
        const std::string&   getResourceName()        const;
        const std::string&   getResourceDescription() const;
        const std::string&   getResourceTexture()     const;
        const core::Vector2i getResourceTextureSize() const;
        const std::string&   getIconTexture()         const;
        const core::Vector2i getIconTextureSize()     const;
        const std::string&   getResourceType()        const;
        int   getMinimumOccurence() const;
        int   getMaximumOccurence() const;
        float getGenerationChance() const;
        int   getPatchSize()        const;
        bool  canBeGenerated(GameObject* object) const;

        void setResourceName(const std::string& name);
        void setResourceDescription(const std::string& description);
        void setResourceTexture(const std::string& texture);
        void setResourceIcon(const std::string& icon_texture);
        void setResourceType(const std::string& resource_type);

        StorageItem asItem() const;

        // Is tile valid for resource placement.
        bool isTileValid(int region_index, int tile_index) const;
        
        // Is region valid for resource generation.
        bool isRegionValid(int region_index) const;
        void placeResource(int region_index, int tile_index) const;
        
        // The resource might not be a patch, such as stone or flint.
        // Perhaps it is a Animal Spot which is only a single resource.
        bool isSingleObject() const;

        bool operator == (const Resource& resource) const;
        bool operator != (const Resource& resource) const;
};
}

extern std::vector <iso::Resource> RESOURCE_TABLE;