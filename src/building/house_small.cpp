#include "house_small.hpp"

using namespace iso;

HouseSmall::HouseSmall() 
    : Building(BUILDINGSIZE128X128, "building_house", "Small House", "icon_default", 3, VECTOR2X2, VECTOR1X1, ResourceCollection(0, 0, 0)) 
{}

HouseSmall::~HouseSmall() {

}