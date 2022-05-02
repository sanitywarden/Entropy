#include "animal_spot.hpp"

using namespace iso;

AnimalSpot::AnimalSpot()
    : Building(BUILDINGSIZE128X128, "animalspot_deer", "Animal Spot", "icon_default", 8, VECTOR2X2, VECTOR1X1, ResourceCollection(0, 0, 0))
{}

AnimalSpot::~AnimalSpot() {
    
}