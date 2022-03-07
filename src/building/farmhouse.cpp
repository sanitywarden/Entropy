#include "farmhouse.hpp"

using namespace iso;

Farmhouse::Farmhouse() : Building(VECTOR0X0, VECTOR0X0, BUILDINGSIZE64X64, "building_farmland", "Farmhouse", 4, VECTOR1X1, ResourceCollection(0, 0, 0)) {
    
}

Farmhouse::~Farmhouse() {

}