#include "house_small.hpp"

using namespace iso;

HouseSmall::HouseSmall() 
    : Building(BUILDINGSIZE128X128, "building_house", "Small House", "icon_building_house", 3, VECTOR2X2, VECTOR1X1) 
{
    this->loadResourceCost();
}

HouseSmall::~HouseSmall() {

}