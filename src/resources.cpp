#include "resources.hpp"

using namespace iso;

ResourceCollection& ResourceCollection::operator+= (const ResourceCollection& cost) {
    this->wood  += cost.wood;
    this->stone += cost.stone;
    this->gold  += cost.gold;

    return *this;
}

ResourceCollection& ResourceCollection::operator-= (const ResourceCollection& cost) {
    this->wood  -= cost.wood;
    this->stone -= cost.stone;
    this->gold  -= cost.gold;

    return *this;
}

ResourceCollection ResourceCollection::operator+ (const ResourceCollection& cost) {
    ResourceCollection temporary(*this);
    return temporary -= cost;
}

ResourceCollection ResourceCollection::operator- (const ResourceCollection& cost) {
    ResourceCollection temporary(*this);
    return temporary -= cost;
}

bool ResourceCollection::operator> (const ResourceCollection& cost) const {
    return (
        this->wood  > cost.wood  &&
        this->stone > cost.stone &&
        this->gold  > cost.gold
    );
}

bool ResourceCollection::operator>= (const ResourceCollection& cost) const {
    return (
        this->wood  >= cost.wood  &&
        this->stone >= cost.stone &&
        this->gold  >= cost.gold
    );
}

bool ResourceCollection::operator< (const ResourceCollection& cost) const {
    // Reuse the '>' operator by negating it's result.
    return !(*this > cost);
}

bool ResourceCollection::operator<= (const ResourceCollection& cost) const {
    return (
        this->wood  <= cost.wood  &&
        this->stone <= cost.stone &&
        this->gold  <= cost.gold
    );
}