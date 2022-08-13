#include "player.hpp"
#include "generationSettings.hpp"
#include "luadriver.hpp"

#include <iostream>

using namespace iso;

Player::Player() {
    this->is_human     = false;
    this->country_name = "*";
    this->capital_region = -1;
    this->player_id      = -1;
    this->spawn_spot_index = -1;
}

Player::~Player() 
{}   

void Player::addOwnedRegion(int region_index) {
    this->owned_regions.push_back(region_index);
}

void Player::removeOwnedRegion(int region_index) { 
    for(auto iterator = this->owned_regions.begin(); iterator != this->owned_regions.end(); ++iterator) {
        if(*iterator == region_index) {
            this->owned_regions.erase(iterator);
            break;
        }
    }
}

const std::vector <int>& Player::readOwnedRegions() {
    return this->owned_regions;
}

void Player::setHuman(bool is_human) {
    this->is_human = is_human;
}

bool Player::isHuman() const {
    return this->is_human;
}

int Player::getCapital() const {
    return this->capital_region;
}

bool Player::hasCapital() const {
    return this->capital_region != -1;
}

void Player::setCapital(int region_index) {
    this->capital_region = region_index;
}

const sf::Color Player::getTeamColourFull() const {
    return this->team_colour;
}

const sf::Color Player::getTeamColourTransparent() const {
    return sf::Color(this->team_colour.r, this->team_colour.g, this->team_colour.b, 191);
}

void Player::setTeamColour(const sf::Color& team_colour) {
    this->team_colour = team_colour;
}

void Player::setCountryName(const std::string& name) {
    this->country_name = name;
}

const std::string& Player::getCountryName() {
    return this->country_name;
}

// void Player::addUnit(std::shared_ptr <Unit> unit) {
//     this->units.push_back(unit);
// }

// Unit* Player::getUnit(int unit_id) {
//     for(auto& unit : this->units) {
//         if(unit.get()->getID() == unit_id)
//             return unit.get();
//     }

//     return nullptr;
// }

// Unit* Player::getUnit(std::string unit_name) {
//     for(auto& unit : this->units) {
//         if(unit.get()->getUnitName() == unit_name)
//             return unit.get();
//     }

//     return nullptr;
// }

// void Player::removeUnit(int unit_id) {
//     for(auto it = this->units.begin(); it != units.end(); ++it) {
//         auto unit = (*it).get();
//         if(unit->getID() == unit_id) {
//             this->units.erase(it);
//             return;
//         }
//     }
// }

// bool Player::hasUnit(int unit_id) const {
//     for(auto& unit : this->units)
//         if(unit.get()->getID() == unit_id)
//             return true;
//     return false;
// }

int Player::empireSize() const {
    return this->owned_regions.size();
}

int Player::getID() const {
    return this->player_id;
}

bool Player::discoveredRegion(int index) const {
    return std::find(this->discovered_regions.begin(), this->discovered_regions.end(), index) != this->discovered_regions.end();    
}

bool Player::ownsRegion(int index) const {
    return std::find(this->owned_regions.begin(), this->owned_regions.end(), index) != this->owned_regions.end();
}

// void Player::addQuest(std::shared_ptr<Quest> quest) {
//     this->current_quests.push_back(quest);
// }

// void Player::removeQuest(std::shared_ptr<Quest> quest) {
//     for(auto it = this->current_quests.begin(); it != this->current_quests.end(); ++it) {
//         if((*it).get()->quest_code_id == quest.get()->quest_code_id) {
//             this->current_quests.erase(it);
//             return;
//         }
//     }
// }

// void Player::finishQuest(std::shared_ptr<Quest> quest) {
//     quest.get()->onQuestFinish();
// } 

// bool Player::hasQuest(std::shared_ptr<Quest> quest) const {
//      for(auto it = this->current_quests.begin(); it != this->current_quests.end(); ++it) {
//         if((*it).get()->quest_code_id == quest.get()->quest_code_id) {
//             return true;
//         }
//     }

//     return false;
// }

bool Player::canAffordBuilding(const Region& region, const Building& building) const {
    if(!game_settings.buildingCostEnabled())
        return true;
    
    // TODO: Write implementation.
    return false;
}

bool Player::placeBuildingCheck(Region& region, Building building, sf::Vector2i grid) const {
    if(region.isBuildingPositionValid(building, grid) && this->canAffordBuilding(region, building)) {
        this->placeBuilding(region, building, grid);

        if(game_settings.buildingCostEnabled())
            region.removeBuildingCost(building);
        
        return true;
    }

    return false;
}

void Player::placeBuilding(Region& region, Building building, sf::Vector2i grid) const {
    auto index = game_settings.calculateRegionIndex(grid);
    const auto& tile = region.map.at(index);  
    building.object_position = tile.getPosition();

    const int a1_w = 0; 
    const int a1_h = game_settings.tileSize().y; 
    const int r_w  = game_settings.tileSize().x / 2;
    const int r_h  = game_settings.tileSize().y;    
    
    // n takes into account only one dimension,
    // because non-square buildings are not currently supported.
    const int n = building.getBuildingArea().x;                    
    
    // Here you adjust the origin of buildings with sizes of n > 0.
    // Texture size scale are arithmetic series.
    auto offset = sf::Vector3f(0, 0, 0);  
    if(n > 0) {
        offset = sf::Vector3f(
            -(a1_w + (n - 1) * r_w),
            -(a1_h + (n - 1) * r_h),
            0
        );
    }

    building.object_position += offset;

    for(int y = 0; y < building.getBuildingArea().y; y++) {
        for(int x = 0; x < building.getBuildingArea().x; x++) {
            auto loop_index = index + game_settings.calculateRegionIndex(x, y);
            region.buildings[loop_index] = BUILDING_EMPTY;
            region.buildings[loop_index].setBuildingName(building.getBuildingName());
        }
    }

    region.buildings[index] = building;
}

void Player::destroyBuilding(Region& region, sf::Vector2i grid) const {
    auto index = game_settings.calculateRegionIndex(grid);

    if(!region.buildingExistsAt(index))
        return;

    auto building = region.getBuildingAt(index);

    for(int y = 0; building.getBuildingArea().y; y++) {
        for(int x = 0; x < building.getBuildingArea().x; x++) {
            auto loop_index = index + game_settings.calculateRegionIndex(x, y);
            region.buildings.erase(loop_index);
        }
    }
}
