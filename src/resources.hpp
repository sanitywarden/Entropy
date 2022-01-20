#ifndef _RESOURCE_COST_HPP_
#define _RESOURCE_COST_HPP_

namespace iso {
    enum class ResourceType {
        RESOURCE_NONE,
        RESOURCE_WOOD,
        RESOURCE_STONE,
        RESOURCE_GOLD
    };

    class ResourceCollection {
        public:
            int wood;
            int stone;
            int gold;

        public:
            ResourceCollection() : wood(0), stone(0), gold(0) {}
            ResourceCollection(int wood, int stone, int gold) : wood(wood), stone(stone), gold(gold) {}

            ResourceCollection& operator -= (const ResourceCollection& cost);
            ResourceCollection  operator -  (const ResourceCollection& cost);  
            ResourceCollection& operator += (const ResourceCollection& cost);
            ResourceCollection  operator +  (const ResourceCollection& cost);  
            bool operator >  (const ResourceCollection& cost) const;
            bool operator >= (const ResourceCollection& cost) const;
            bool operator <  (const ResourceCollection& cost) const;
            bool operator <= (const ResourceCollection& cost) const;
    };
}

#endif