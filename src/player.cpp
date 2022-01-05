#include "player.hpp"

#include <iostream>

using namespace iso;

Player::Player() {
    this->is_human = false;
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

bool Player::isHuman() {
    return this->is_human;
}

void Player::addResource(Resource resource, int quantity) {
    switch(resource) {
        default:
            break;

        case Resource::RESOURCE_WOOD:
            this->resources.wood += quantity;
            break;

        case Resource::RESOURCE_STONE:
            this->resources.stone += quantity;
            break;

        case Resource::RESOURCE_GOLD:
            this->resources.gold += quantity;
            break;
    }
}

void Player::addResources(ResourceCost resource) {
    this->resources += resource;
}

int Player::getResourceQuantity(Resource resource) {
    switch(resource) {
        default:
            return 0;

        case Resource::RESOURCE_WOOD:
            return this->resources.wood;

        case Resource::RESOURCE_STONE:
            return this->resources.wood;
    
        case Resource::RESOURCE_GOLD:
            return this->resources.gold;
    }
}

bool Player::isBuildingAffordable(const Building building) {
    return (this->resources >= building.getBuildingCost());
}

/* Remove the resources needed to construct provided building from player's resource pool. */
void Player::removeBuildingCost(const Building building) {
    this->resources -= building.getBuildingCost();
}

int Player::getCapital() {
    return this->owned_regions[0];
}

const sf::Color& Player::getTeamColour() {
    return this->team_colour;
}