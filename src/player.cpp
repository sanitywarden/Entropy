#include "player.hpp"

#include <iostream>

using namespace iso;

Player::Player() {
    this->is_human     = false;
    this->country_name = "*";
    this->capital_region = -1;
    this->player_id      = -1;
}

Player::~Player() {

}   

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

void Player::addUnit(std::shared_ptr <Unit> unit) {
    this->units.push_back(unit);
}

Unit* Player::getUnit(int unit_id) {
    for(auto& unit : this->units) {
        if(unit.get()->getID() == unit_id)
            return unit.get();
    }

    return nullptr;
}

Unit* Player::getUnit(std::string unit_name) {
    for(auto& unit : this->units) {
        if(unit.get()->getUnitName() == unit_name)
            return unit.get();
    }

    return nullptr;
}

void Player::removeUnit(int unit_id) {
    for(auto it = this->units.begin(); it != units.end(); ++it) {
        auto unit = (*it).get();
        if(unit->getID() == unit_id) {
            this->units.erase(it);
            return;
        }
    }
}

bool Player::hasUnit(int unit_id) const {
    for(auto& unit : this->units)
        if(unit.get()->getID() == unit_id)
            return true;
    return false;
}

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

void Player::addQuest(std::shared_ptr<Quest> quest) {
    this->current_quests.push_back(quest);
}

void Player::removeQuest(std::shared_ptr<Quest> quest) {
    for(auto it = this->current_quests.begin(); it != this->current_quests.end(); ++it) {
        if((*it).get()->quest_code_id == quest.get()->quest_code_id) {
            this->current_quests.erase(it);
            return;
        }
    }
}

void Player::finishQuest(std::shared_ptr<Quest> quest) {
    quest.get()->onQuestFinish();
} 

bool Player::hasQuest(std::shared_ptr<Quest> quest) const {
     for(auto it = this->current_quests.begin(); it != this->current_quests.end(); ++it) {
        if((*it).get()->quest_code_id == quest.get()->quest_code_id) {
            return true;
        }
    }

    return false;
}