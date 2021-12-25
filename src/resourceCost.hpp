#ifndef _RESOURCE_COST_HPP_
#define _RESOURCE_COST_HPP_

namespace iso {
    enum class Resource {
        RESOURCE_NONE,
        RESOURCE_WOOD,
        RESOURCE_STONE,
        RESOURCE_GOLD
    };

    class ResourceCost {
        public:
            int wood;
            int stone;
            int gold;

        public:
            ResourceCost() : wood(0), stone(0), gold(0) {}
            ResourceCost(int wood, int stone, int gold) : wood(wood), stone(stone), gold(gold) {}

            ResourceCost& operator -= (const ResourceCost& cost);
            ResourceCost  operator -  (const ResourceCost& cost);  
            ResourceCost& operator += (const ResourceCost& cost);
            ResourceCost  operator +  (const ResourceCost& cost);  
            bool operator >  (const ResourceCost& cost);
            bool operator >= (const ResourceCost& cost);
            bool operator <  (const ResourceCost& cost);
            bool operator <= (const ResourceCost& cost);
    };
}

#endif