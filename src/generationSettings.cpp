#include "generationSettings.hpp"
#include "globalutilities.hpp"

#include <iostream>
#include <fstream>

using namespace iso;

WorldData::WorldData() {
    this->world_panel_size   = 128;
    this->region_tile_offset = sf::Vector2f(100, 100);
    this->region_tile_size   = sf::Vector2f(64, 32);
    this->region_size        = this->world_panel_size;

    this->loadSettingsFromFile();

    this->world_margin_island   = this->world_size / 24;
    this->world_margin_poles    = this->world_size / 10;
    this->world_river_quantity  = this->world_size / 8;
    this->world_river_scan_size = this->world_size / 8;

    this->player_quantity = this->world_size / 10;

    this->world_noise.size    = sf::Vector2f(this->world_size, this->world_size);
    this->world_forest.size   = sf::Vector2f(this->world_size, this->world_size);
    this->world_moisture.size = sf::Vector2f(this->world_size, this->world_size);
    this->world_gradient.size = sf::Vector2f(this->world_size, this->world_size);
}   

WorldData::~WorldData() {

}

void WorldData::loadSettingsFromFile() {
    const std::string config_file_path = "./config/world_generation.config";
    std::fstream config_file(config_file_path);

    if(!config_file.good()) {
        std::cout << "[Generation Settings]: Could not open config file: " << config_file_path << ".\n";
        exit(1);
    }

    std::cout << "[Generation Settings]: Reading world generation properties.\n";

    std::string line_delimiter  = ",";   // What char marks that a line ends.
    std::string read_value_from = ":";   // What char marks that value is afterwards.
    char comment_indicator = '#';        // What char marks a comment.
    int  ascii_empty_line_indicator = 0; // What value marks that a line is empty (ASCII NULL).
    
    std::string line_content;

    while(std::getline(config_file, line_content)) {
        if(line_content[0] == comment_indicator || (int)line_content[0] == ascii_empty_line_indicator)
            continue;

        auto index = find(line_content, read_value_from);
        std::string property_name  = readBefore(line_content, read_value_from);  
        std::string property_value = read(line_content, index + 1, line_content.length() - 1);

        // Well, this is bad.
        // Perhaps you could hold names, and values of properties in a map, and retrieve them by specific functions on demand.

        if(property_name == "WORLDSIZE") {
            this->world_size = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_MARGIN") {
            this->world_margin_island = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_TERRAIN") {
            this->world_noise_terrain = std::stof(property_value);
            continue;
        }

        if(property_name == "WORLD_FOREST") {
            this->world_noise_forest = std::stof(property_value);
            continue;
        }

        if(property_name == "REGION_RESOURCE_CHANCE_FLINT") {
            this->resource_flint_chance = std::stof(property_value);
            continue;
        }        

        if(property_name == "REGION_RESOURCE_CHANCE_STONE") {
            this->resource_stone_chance = std::stof(property_value);
            continue;
        } 

        if(property_name == "REGION_RESOURCE_RADIUS_DIV_FLINT") {
            this->resource_flint_radius = this->region_size / std::stoi(property_value);
            continue;
        }

        if(property_name == "REGION_RESOURCE_RADIUS_DIV_STONE") {
            this->resource_stone_radius = this->region_size / std::stoi(property_value);
            continue;
        }

        if(property_name == "REGION_RESOURCE_MINIMUM_WOOD") {
            this->tree_noise_minimum = std::stof(property_value);
            continue;
        } 

        if(property_name == "REGION_RESOURCE_MINIMUM_FLINT") {
            this->flint_noise_minimum = std::stof(property_value);
            continue;
        } 

        if(property_name == "REGION_RESOURCE_MINIMUM_STONE") {
            this->stone_noise_minimum = std::stof(property_value);
            continue;
        } 

        // WORLD NOISE.

        if(property_name == "WORLD_NOISE_O") {
            this->world_noise.octaves = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_NOISE_P") {
            this->world_noise.persistence = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_NOISE_B") {
            this->world_noise.bias = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_NOISE_M") {
            this->world_noise.multiplier = std::stof(property_value);
            continue;
        }

        // MOISTURE NOISE.

        if(property_name == "WORLD_MOISTURE_NOISE_O") {
            this->world_moisture.octaves = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_MOISTURE_NOISE_P") {
            this->world_moisture.persistence = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_MOISTURE_NOISE_B") {
            this->world_moisture.bias = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_MOISTURE_NOISE_M") {
            this->world_moisture.multiplier = std::stof(property_value);
            continue;
        }

        // ISLAND NOISE.
        
        if(property_name == "WORLD_ISLAND_NOISE_O") {
            this->world_gradient.octaves = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_ISLAND_NOISE_P") {
            this->world_gradient.persistence = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_ISLAND_NOISE_B") {
            this->world_gradient.bias = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_ISLAND_NOISE_M") {
            this->world_gradient.multiplier = std::stof(property_value);
            continue;
        }

        // FOREST NOISE.

        if(property_name == "WORLD_FOREST_NOISE_O") {
            this->world_forest.octaves = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_FOREST_NOISE_P") {
            this->world_forest.persistence = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_FOREST_NOISE_B") {
            this->world_forest.bias = std::stoi(property_value);
            continue;
        }

        if(property_name == "WORLD_FOREST_NOISE_M") {
            this->world_forest.multiplier = std::stof(property_value);
            continue;
        }
    }

    config_file.close();
}

int WorldData::getWorldSize() const { 
    return this->world_size * this->world_size;
}

int WorldData::getWorldWidth() const {
    return this->world_size;
}

int WorldData::getWorldMargin() const {
    return this->world_margin_island;
}

int WorldData::getWorldMarginPolar() const {
    return this->world_margin_poles;
}

int WorldData::getWorldRiverQuantity() const {
    return this->world_river_quantity;
}

int WorldData::getWorldRiverScan() const {
    return this->world_river_scan_size;
}

float WorldData::getWorldTerrainMin() const {
    return this->world_noise_terrain;
}

float WorldData::getWorldForestMin() const {
    return this->world_noise_forest;
}

int WorldData::calculateWorldIndex(int x, int y) const {
    return y * this->world_size + x;
}

bool WorldData::inWorldBounds(int index) const {
    return (index >= 0 && index < this->getWorldSize());
}

bool WorldData::inWorldBounds(sf::Vector2i grid_position) const {
    return (grid_position.x >= 0 && grid_position.y >= 0 && grid_position.x < this->getWorldWidth() && grid_position.y < this->getWorldWidth());
}

NoiseSettings WorldData::getWorldNoiseSettings() const {
    return world_noise;
}

NoiseSettings WorldData::getWorldMoistureSettings() const {
    return world_moisture;
}

NoiseSettings WorldData::getWorldGradientSettings() const {
    return world_gradient;
}

NoiseSettings WorldData::getWorldForestSettings() const {
    return world_forest;
}

int WorldData::getRegionSize() const {
    return this->region_size * this->region_size;
}

int WorldData::getRegionWidth() const {
    return this->region_size;
}

int WorldData::calculateRegionIndex(int x, int y) const {
    return y * this->region_size + x;
}

bool WorldData::inRegionBounds(int index) const {
    return (index >= 0 && index < this->getRegionSize());
}

bool WorldData::inRegionBounds(sf::Vector2i grid_position) const {
    return (
        grid_position.x >= 0 && grid_position.x < this->getRegionWidth() && 
        grid_position.y >= 0 && grid_position.y < this->getRegionWidth()
    );
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

float WorldData::getRegionFlintChance() const {
    return this->resource_flint_chance;
}

float WorldData::getRegionStoneChance() const {
    return this->resource_flint_chance;
}

int WorldData::getRegionFlintRadius() const {
    return this->resource_flint_radius;
}

int WorldData::getRegionStoneRadius() const {
    return this->resource_stone_radius;
}

float WorldData::getRegionTreeMin() const {
    return this->tree_noise_minimum;
}

float WorldData::getRegionFlintMin() const {
    return this->flint_noise_minimum;
}

float WorldData::getRegionStoneMin() const {
    return this->stone_noise_minimum;
}

int WorldData::panelSize() const {
    return this->world_panel_size;
}

sf::Vector2f WorldData::tileSize() const {
    return this->region_tile_size;
}

sf::Vector2f WorldData::tileOffset() const {
    return this->region_tile_offset;
}

int WorldData::getPlayerQuantity() const {
    return this->player_quantity;
}