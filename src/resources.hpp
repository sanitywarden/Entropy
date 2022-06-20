#ifndef _RESOURCE_COST_HPP_
#define _RESOURCE_COST_HPP_

#include <vector>
#include <string>

namespace iso {
    enum class ResourceType {
        TYPE_UNCATEGORISED,
        TYPE_FOOD,
        TYPE_BASE_MATERIAL,
    };

    class Resource {
        private:
            int quantity;
            std::string name;
            std::string icon_name;
            ResourceType type;

        public:
            Resource();
            Resource(std::string resource_name, int quantity);
            Resource(std::string resource_name, std::string resource_icon, ResourceType type, int quantity);
            ~Resource();

            Resource& operator -= (const Resource& cost);
            Resource  operator -  (const Resource& cost);  
            Resource& operator += (const Resource& cost);
            Resource  operator +  (const Resource& cost);  
            bool operator >  (const Resource& cost) const;
            bool operator >= (const Resource& cost) const;
            bool operator <  (const Resource& cost) const;
            bool operator <= (const Resource& cost) const;

            void setQuantity(int quantity);
            void incrementQuantity();
            int getQuantity() const;
            ResourceType getType() const;
            std::string getName() const;
            std::string getIcon() const;
    };

    const Resource RESOURCE_EMPTY  ("None",    "default"          , ResourceType::TYPE_UNCATEGORISED, 0);
    const Resource RESOURCE_WOOD   ("Wood",    "icon_item_wood"   , ResourceType::TYPE_BASE_MATERIAL, 0);
    const Resource RESOURCE_STONE  ("Stone",   "icon_item_stone"  , ResourceType::TYPE_BASE_MATERIAL, 0);
    const Resource RESOURCE_FLINT  ("Flint",   "icon_item_flint"  , ResourceType::TYPE_BASE_MATERIAL, 0);
    const Resource RESOURCE_GRAIN  ("Grain",   "icon_item_grain"  , ResourceType::TYPE_FOOD         , 0);
    const Resource RESOURCE_MEAT   ("Meat",    "icon_item_meat"   , ResourceType::TYPE_FOOD         , 0);
    const Resource RESOURCE_LEATHER("Leather", "icon_item_leather", ResourceType::TYPE_BASE_MATERIAL, 0);
    const Resource RESOURCE_WATER  ("Water",   "icon_item_water"  , ResourceType::TYPE_FOOD         , 0);
    const Resource RESOURCE_TOOLS  ("Tools",   "icon_item_tools"  , ResourceType::TYPE_BASE_MATERIAL, 0);

    const static std::vector <Resource> RESOURCE_LOOKUP_TABLE = {
        RESOURCE_WOOD,
        RESOURCE_STONE,
        RESOURCE_FLINT,
        RESOURCE_GRAIN,
        RESOURCE_MEAT,
        RESOURCE_LEATHER,
        RESOURCE_WATER,
        RESOURCE_TOOLS,
    };
}

#endif