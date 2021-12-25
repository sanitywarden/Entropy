#include "resourceCost.hpp"

using namespace iso;

ResourceCost& ResourceCost::operator+= (const ResourceCost& cost) {
    this->wood  += cost.wood;
    this->stone += cost.stone;
    this->gold  += cost.gold;

    return *this;
}

ResourceCost& ResourceCost::operator-= (const ResourceCost& cost) {
    this->wood  -= cost.wood;
    this->stone -= cost.stone;
    this->gold  -= cost.gold;

    return *this;
}

ResourceCost ResourceCost::operator+ (const ResourceCost& cost) {
    ResourceCost temporary(*this);
    return temporary -= cost;
}

ResourceCost ResourceCost::operator- (const ResourceCost& cost) {
    ResourceCost temporary(*this);
    return temporary -= cost;
}

bool ResourceCost::operator> (const ResourceCost& cost) {
    return (
        this->wood  > cost.wood  &&
        this->stone > cost.stone &&
        this->gold  > cost.gold
    );
}

bool ResourceCost::operator>= (const ResourceCost& cost) {
    return (
        this->wood  >= cost.wood  &&
        this->stone >= cost.stone &&
        this->gold  >= cost.gold
    );
}

bool ResourceCost::operator< (const ResourceCost& cost) {
    // Reuse the '>' operator by negating it's result.
    return !(*this > cost);
}

bool ResourceCost::operator<= (const ResourceCost& cost) {
    return (
        this->wood  <= cost.wood  &&
        this->stone <= cost.stone &&
        this->gold  <= cost.gold
    );
}