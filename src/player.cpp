#include "player.hpp"
#include "worldData.hpp"
#include "globalutilities.hpp"
#include "simulationManager.hpp"
#include "regionmap.hpp"

#include <iostream>

namespace iso {
Player::Player(PlayerData pdata, CountryData cdata)
    : player_data(pdata), country_data(cdata)
{}

Player::~Player() 
{}   

bool Player::canAffordBuilding(const Region& region, const Building& building) const {
    if(!world_data.building_cost_enabled)
        return true;
    
    int can_afford = 0;
    for(auto& cost : building.getBuildingCost()) {
        if(region.itemExists(cost) && region.itemQuantity(cost) >= cost.getAmount())
            can_afford++;
    }

    return can_afford == building.getBuildingCost().size();
}

bool Player::placeBuildingCheck(Region& region, Building building, core::Vector2i grid) {
    if(region.isBuildingPositionValid(building, grid) && this->canAffordBuilding(region, building)) {
        this->placeBuilding(region, building, grid);

        if(world_data.building_cost_enabled)
            region.removeBuildingCost(building);
        
        return true;
    }

    return false;
}

void Player::placeBuilding(Region& region, Building building, core::Vector2i grid) {    
    if(building == BUILDING_EMPTY) {
        printError("Player::placeBuilding()", "Placed empty building");
        return;
    }

    auto index = calculateRegionIndex(grid);
    const auto& tile = region.getTileAtIndex(index);  
    building.object_position = tile.getPosition();

    const int a1_w = 0; 
    const int a1_h = tileSize().y; 
    const int r_w  = tileSize().x / 2;
    const int r_h  = tileSize().y;    
    
    // n takes into account only one dimension,
    // because non-square buildings are not currently supported.
    const int n = building.getBuildingArea().x;                    
    
    // Here you adjust the origin of buildings with sizes of n > 0.
    // Texture size scale are arithmetic series.
    auto offset = core::Vector3i(0, 0, 0);  
    if(n > 0) {
        offset = core::Vector3i(
            -(a1_w + (n - 1) * r_w),
            -(a1_h + (n - 1) * r_h),
            0
        );
    }

    building.object_position += offset;

    for(int y = 0; y < building.getBuildingArea().y; y++) {
        for(int x = 0; x < building.getBuildingArea().x; x++) {
            auto loop_index = index + calculateRegionIndex(x, y);
            region.buildings[loop_index] = BUILDING_EMPTY;
            region.buildings[loop_index].index = loop_index;
        }
    }

    region.buildings[index] = building;
    region.buildings[index].index = index;

    handleScriptableEvent("onBuildingConstruct");
    auto regionmap = (Regionmap*)game_manager.gamestate.getGamestate();
    regionmap->recalculate_tree_mesh = true;
}

void Player::destroyBuilding(Region& region, core::Vector2i grid) {
    auto index = calculateRegionIndex(grid);
    if(!region.buildingExistsAtIndex(index))
        return;

    const auto& building = region.getBuildingAtIndex(index);
    if(building == BUILDING_EMPTY)
        return;

    for(int y = 0; y < building.getBuildingArea().y; y++) {
        for(int x = 0; x < building.getBuildingArea().x; x++) {
            auto loop_index = index + calculateRegionIndex(x, y);
            region.buildings.erase(loop_index);
        }
    }

    handleScriptableEvent("onBuildingDestroy");
    auto regionmap = (Regionmap*)game_manager.gamestate.getGamestate();
    regionmap->recalculate_tree_mesh = true;
}

void Player::addOwnedRegion(int index) {
    if(!inWorldBounds(index)) {
        printError("Player::addOwnedRegion()", "Region index out of bounds");
        return;
    }

    auto& region = game_manager.world_map.at(index);
    region.setOwner(this->getID());
    this->country_data.owned_regions.push_back(index);
}

void Player::removeOwnedRegion(int index) { 
    for(auto iterator = this->country_data.owned_regions.begin(); iterator != this->country_data.owned_regions.end(); ++iterator) {
        if(*iterator == index) {
            this->country_data.owned_regions.erase(iterator);
            break;
        }
    }

    auto& region = game_manager.world_map.at(index);
    region.setOwner(-1);
}

bool Player::ownsRegion(int index) const {
    return std::find(this->country_data.owned_regions.begin(), this->country_data.owned_regions.end(), index) != this->country_data.owned_regions.end();
}

Region& Player::getOwnedRegion(int index) {
    if(!this->ownsRegion(index))    
        printError("Player::getOwnedRegion()", "Requested region not owned by the player");
    
    if(!inWorldBounds(index))
        printError("Player::getOwnedRegion()", "Region index out of bounds");
    return game_manager.world_map.at(index);
}

void Player::addKnownRegion(int index) {
    this->country_data.known_regions.push_back(index);
}

bool Player::discoveredRegion(int index) const {
    return std::find(this->country_data.known_regions.begin(), this->country_data.known_regions.end(), index) != this->country_data.known_regions.end();    
}

int Player::countrySize() const {
    return this->country_data.owned_regions.size();
}

int Player::getCapital() const {
    if(!this->hasCapital()) {
        printError("Player::getCapital()", "Capital index is -1");
        return -1;
    }
    return this->country_data.capital;
}

bool Player::hasCapital() const {
    return this->country_data.capital != -1;
}

void Player::setCapital(int index) {
    if(!inWorldBounds(index)) {
        printError("Player::setCapital()", "Region index out of bounds");
        return;
    }
    this->country_data.capital = index;
}

bool Player::isHuman() const {
    return this->player_data.is_human;
}

void Player::setHuman(bool is_human) {
    this->player_data.is_human = is_human;
}

core::Colour Player::getCountryColourFull() const {
    return this->country_data.map_colour;
}

core::Colour Player::getCountryColourTransparent() const {
    auto colour = this->getCountryColourFull();
    return core::Colour(colour.r, colour.g, colour.b, 127);
}

void Player::setCountryColour(core::Colour colour) {
    this->country_data.map_colour = colour;
}

void Player::setCountryName(const std::string& name) {
    this->country_data.country_name = name;
}

const std::string& Player::getCountryName() const {
    return this->country_data.country_name;
}

void Player::setCultureGroup(const std::string& culture_file) {
    this->country_data.culture_filename = culture_file;
}

const std::string& Player::getCultureGroup() const {
    return this->country_data.culture_name;
}

int Player::getID() const {
    return this->player_data.id;
}

int Player::getInitialSpawn() const {
    return this->country_data.initial_spawn;
}
}