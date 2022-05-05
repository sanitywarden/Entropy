#include "path_stone.hpp"

using namespace iso;

PathStone::PathStone() 
    : Building(BUILDINGSIZE64X64, "path_stone_point", "Stone path", "icon_path_stone", 2, VECTOR1X1, VECTOR1X1) 
{}

PathStone::~PathStone() {

}