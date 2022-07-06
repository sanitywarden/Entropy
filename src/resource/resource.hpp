#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

#include <vector>
#include <string>

#include "gameObject.hpp"

namespace iso {
    // Classification of resources.
    enum class ResourceType {
        TYPE_UNCATEGORISED,
        TYPE_PLANT_RESOURCE, // Cotton, Silk etc.
        TYPE_PLANT_FOOD,     // Apple, Pear  etc.
        TYPE_ANIMAL_FOOD,    // Meat
        TYPE_BASE_MATERIAL,  // Wood, Stone  etc.
        TYPE_RAW_MATERIAL,   // Copper, Tin  etc.
    };

    // Resource is a special commodity that may be generated in a region upon visiting.
    // They may be common or rare.
    // Certain resources exist only in specific areas of the world. 
    class Resource {
        public:
            int          min_occurence; // How many patches of this resource should be generated.
            int          max_occurence; // How many patches of this resource may be generated.
            std::string  resource_name; // Readable resource name.
            std::string  resource_icon; // Resoruce icon.
            ResourceType resource_type;
            
            Resource();
            Resource(const std::string& name, ResourceType type);
            Resource(const std::string& name, ResourceType type, const std::string& icon);
            Resource(const std::string& name, ResourceType type, const std::string& icon, int min, int max);
            ~Resource();

            virtual bool isGenerationSpotValid(GameObject* object, int index) const { return false; }
            virtual void placeResource(GameObject* object, int index) const { return; }
            
            // The resource might not be a patch, such as stone or flint.
            // Perhaps it is a Animal Spot which is only a single resource.
            virtual bool isSingleObject() const { return false; }
    };
}

#endif