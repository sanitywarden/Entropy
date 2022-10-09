#pragma once

#include "types.hpp"

#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

namespace iso {
struct ItemData {
    std::string    filename;     // Path to the definition file of the item.
    std::string    name;         // Human readable name.
    std::string    description;  // Human readable description.
    std::string    icon_texture; // Icon texture.
    core::Vector2i icon_size;    // Icon size in pixels.
    std::string    type;         // Item type.
    int            amount;       // Amount of the item.
}; 
    
class Resource;

// Item is a commodity held in a region's stockpile.
// Item may be extracted from a resource or created from other items.
class StorageItem {
    protected:
        ItemData data;
    
    public:
        StorageItem();
        StorageItem(const StorageItem& item);
        StorageItem(const ItemData& data);
        ~StorageItem();
        
        StorageItem& operator -= (const StorageItem& item);
        StorageItem  operator -  (const StorageItem& item);  
        StorageItem& operator += (const StorageItem& item);
        StorageItem  operator +  (const StorageItem& item);  
        bool operator >  (const StorageItem& item) const;
        bool operator >= (const StorageItem& item) const;
        bool operator <  (const StorageItem& item) const;
        bool operator <= (const StorageItem& item) const;
        StorageItem& operator = (const Resource& resource);

        const std::string&   getDefinitionFilename() const;
        const std::string&   getItemName()           const;
        const std::string&   getItemDescription()    const;
        const std::string&   getIconTexture()        const;
        const core::Vector2i getIconTextureSize()    const;
        const std::string&   getItemType()           const;
        int                  getAmount()             const;

        void setAmount(int amount);
};
}

extern std::vector <iso::StorageItem> ITEM_TABLE;
