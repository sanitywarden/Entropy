#include "quarry.hpp"

using namespace iso;

Quarry::Quarry() : Building(VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_quarry", "Quarry", 5, VECTOR1X1, ResourceCollection(0, 0, 0)) {
    
}

Quarry::~Quarry() {

}