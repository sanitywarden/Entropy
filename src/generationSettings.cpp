#include "generationSettings.hpp"
#include "globalutilities.hpp"

using namespace iso;

GameSettings::GameSettings() 
{}

GameSettings::GameSettings(const GameSettings& settings) {
    this->persistence = settings.persistence;
    this->simulation_update_frequency = settings.simulation_update_frequency;
    this->world_width = settings.world_width;
    this->terrain_from = settings.terrain_from;
    this->forest_from = settings.forest_from;
    this->region_width = settings.region_width;
    this->initial_population = settings.initial_population;
    this->population_needs_enabled = settings.population_needs_enabled;
    this->building_cost_enabled = settings.building_cost_enabled;
    this->astar_pathfinding_enabled = settings.astar_pathfinding_enabled;
}

GameSettings::~GameSettings() 
{}

int GameSettings::getWorldSize() const { 
    return this->world_width * this->world_width;
}

int GameSettings::getWorldWidth() const {
    return this->world_width;
}

int GameSettings::calculateWorldIndex(int x, int y) const {
    return y * this->world_width + x;
}

bool GameSettings::inWorldBounds(int index) const {
    return (index >= 0 && index < this->getWorldSize());
}

bool GameSettings::inWorldBounds(sf::Vector2i grid) const {
    return (grid.x >= 0 && grid.y >= 0 && grid.x < this->getWorldWidth() && grid.y < this->getWorldWidth());
}

int GameSettings::getRegionSize() const {
    return this->region_width * this->region_width;
}

int GameSettings::getRegionWidth() const {
    return this->region_width;
}

int GameSettings::calculateRegionIndex(int x, int y) const {
    return y * this->region_width + x;
}

int GameSettings::calculateRegionIndex(sf::Vector2i grid) const {
    return this->calculateRegionIndex(grid.x, grid.y);
}

bool GameSettings::inRegionBounds(int index) const {
    return (index >= 0 && index < this->getRegionSize());
}

bool GameSettings::inRegionBounds(sf::Vector2i grid_position) const {
    return (
        grid_position.x >= 0 && grid_position.x < this->getRegionWidth() && 
        grid_position.y >= 0 && grid_position.y < this->getRegionWidth()
    );
}

bool GameSettings::populationNeedsEnabled() const {
    return this->population_needs_enabled;
}

bool GameSettings::buildingCostEnabled() const {
    return this->building_cost_enabled;
}

bool GameSettings::astarEnabled() const {
    return this->astar_pathfinding_enabled;
}

bool GameSettings::fogOfWarEnabled() const {
    return this->fog_of_war_enabled;
}

int GameSettings::panelSize() const {
    return 128;
}

sf::Vector2f GameSettings::tileSize() const {
    return sf::Vector2f(64, 32);
}

sf::Vector2f GameSettings::tileOffset() const {
    return sf::Vector2f(100, 100);
}

float GameSettings::simulationSpeed() const {
    return this->simulation_update_frequency;
}

int GameSettings::getRegionPersistence() const {
    return this->persistence;
}

bool GameSettings::inSameRow(int index1, int index2) const {
    auto grid1 = tileGridPosition(index1);
    auto grid2 = tileGridPosition(index2);

    return grid1.y == grid2.y; 
}

bool GameSettings::inSameColumn(int index1, int index2) const {
    auto grid1 = tileGridPosition(index1);
    auto grid2 = tileGridPosition(index2);

    return grid1.x == grid2.x; 
}

int GameSettings::getWorldMargin() const {
    return 3;
}

int GameSettings::getWorldMarginPolar() const {
    return 3; 
}

float GameSettings::getWorldTerrainMin() const {
    return this->terrain_from;
}

int GameSettings::getWorldRiverQuantity() const {
    return 5;
}

float GameSettings::getWorldForestMin() const {
    return this->forest_from;
}

float GameSettings::getRegionTreeMin() const {
    return this->forest_from;
}

int GameSettings::getPlayerQuantity() const {
    return 4;
}