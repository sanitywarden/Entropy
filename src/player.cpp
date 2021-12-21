#include "player.hpp"

#include <iostream>

using namespace iso;

PlayerResources::PlayerResources() {
    this->owned_regions.resize(0);
    this->gold = 0;
    this->wood = 0;
    this->food = 0;
}

PlayerResources::~PlayerResources() {

}

Player::Player() {
    this->player_is_human = false;
}

Player::~Player() {

}   

void Player::addOwnedRegion(int region_index) {
    this->resources.owned_regions.push_back(region_index);
}

void Player::removeOwnedRegion(int region_index) { 
    for(auto iterator = this->resources.owned_regions.begin(); iterator != this->resources.owned_regions.end(); ++iterator) {
        if(*iterator == region_index) {
            this->resources.owned_regions.erase(iterator);
            break;
        }
    }
}

void Player::setHuman(bool player_is_human) {
    this->player_is_human = player_is_human;
}

bool Player::isHuman() {
    return this->player_is_human;
}