#include "generationSettings.hpp"

using namespace iso;

WorldData::WorldData() {
    this->world_size = 50;
    this->world_margin_island   = this->world_size / 10;
    this->world_margin_poles    = this->world_size / 10;
    this->world_river_quantity  = this->world_size / 8;
    this->world_river_scan_size = this->world_size / 8;
    this->world_panel_size      = 128;
    
    this->region_size = this->world_panel_size;
    this->region_tile_size.x   = 64;
    this->region_tile_size.y   = 32;
    this->region_tile_offset.x = 100;
    this->region_tile_offset.y = 100;

    this->world_noise_terrain = 0.20f;
    this->world_noise_forest  = 0.70f;

    this->player_quantity = this->world_size / 10;

    this->resource_flint_chance = 0.80f;
    this->resource_stone_chance = 0.20f;

    this->resource_flint_radius = this->region_size / 16;
    this->resource_stone_radius = this->region_size / 32;

    this->tree_noise_minimum  = 0.50f;
    this->flint_noise_minimum = 0.60f;
    this->stone_noise_minimum = 0.60f;
}   

WorldData::~WorldData() {

}

void WorldData::loadSettingsFromFile() {

}

int WorldData::getWorldSize() const { 
    return this->world_size * this->world_size;
}

int WorldData::calculateWorldIndex(int x, int y) const {
    return y * this->world_size + x;
}

bool WorldData::inWorldBounds(int index) const {
    return (index >= 0 && index < this->getWorldSize());
}

NoiseSettings WorldData::getWorldNoiseSettings() const {
    return NoiseSettings(this->world_size, 16, 10, 4, 1.25f);
}

NoiseSettings WorldData::getWorldMoistureSettings() const {
    return NoiseSettings(this->world_size, 8, 8, 4, 0.90f);
}

NoiseSettings WorldData::getWorldGradientSettings() const {
    return NoiseSettings(this->world_size, 16, 4, 4, 1.75f);
}

NoiseSettings WorldData::getWorldForestSettings() const {
    return NoiseSettings(this->world_size, 8, 4, 4, 1.25f);
}

int WorldData::getRegionSize() const {
    return this->region_size * this->region_size;
}

int WorldData::calculateRegionIndex(int x, int y) const {
    return y * this->region_size + x;
}

bool WorldData::inRegionBounds(int index) const {
    return (index >= 0 && index < this->getRegionSize());
}

NoiseSettings WorldData::getRegionForestSettingsDense() const {
    return NoiseSettings(this->region_size, 8, 16, 4, 1.00f);
}

NoiseSettings WorldData::getRegionForestSettingsSparse() const {
    return NoiseSettings(this->region_size, 8, 16, 4, 0.70f);
}

NoiseSettings WorldData::getRegionResourceSettings() const {
    return NoiseSettings(this->region_size, 8, 16, 4, 1.50f);
}