#include "player.hpp"

#include <iostream>

using namespace iso;

Player::Player() {
    this->is_human     = false;
    this->country_name = "*";
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

int Player::getCapital() {
    return this->owned_regions[0];
}

const sf::Color& Player::getTeamColour() {
    return this->team_colour;
}

void Player::setCountryName(std::string name) {
    this->country_name = name;
}

const std::string& Player::getCountryName() {
    return this->country_name;
}
