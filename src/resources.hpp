#ifndef _RESOURCE_COST_HPP_
#define _RESOURCE_COST_HPP_

#include <vector>
#include <string>

namespace iso {
    class Resource {
        private:
            int quantity;
            std::string name;
            std::string icon_name;

        public:
            Resource();
            Resource(std::string resource_name, int quantity);
            Resource(std::string resource_name, std::string resource_icon, int quantity);
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
            std::string getName() const;
            std::string getIcon() const;
    };

    /*
    icon_resource_wood
    icon_resource_stone
    icon_resource_flint
    icon_resource_food
    icon_resource_leather
    */

    const Resource RESOURCE_WOOD   ("Wood",    "icon_default", 0);
    const Resource RESOURCE_STONE  ("Stone",   "icon_default", 0);
    const Resource RESOURCE_FLINT  ("Flint",   "icon_default", 0);
    const Resource RESOURCE_FOOD   ("Food",    "icon_default", 0);
    const Resource RESOURCE_LEATHER("Leather", "icon_default", 0);

    const static std::vector <Resource> RESOURCE_LOOKUP_TABLE = {
        RESOURCE_WOOD,
        RESOURCE_STONE,
        RESOURCE_FLINT,
        RESOURCE_FOOD,
        RESOURCE_LEATHER,
    };
}

#endif