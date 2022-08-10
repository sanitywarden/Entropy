#pragma once

#include <SFML/System/Vector2.hpp>
#include <map>

namespace iso {    
class GameSettings {
    public:
        GameSettings();
        GameSettings(const GameSettings& settings);
        ~GameSettings();

        int persistence;
        int world_persistence;

        float simulation_update_frequency;

        // World

        int world_width;
        float terrain_from;
        float forest_from;
        float temperature_multiplier;
        float moisture_multiplier;

        // Region

        int region_width;
        int initial_population;
        bool population_needs_enabled;
        bool building_cost_enabled;
        bool astar_pathfinding_enabled;
        bool fog_of_war_enabled;    

    public:
        int getWorldSize() const;
        int getWorldWidth() const;
        bool inWorldBounds(int index) const;
        bool inWorldBounds(sf::Vector2i grid) const;
        int calculateWorldIndex(int x, int y) const;
        int calcluateWorldIndex(sf::Vector2i grid) const;

        int getRegionSize() const;
        int getRegionWidth() const;
        int getRegionInitialPopulation() const;
        bool inRegionBounds(int index) const;
        bool inRegionBounds(sf::Vector2i grid) const;
        int calculateRegionIndex(int x, int y) const;
        int calculateRegionIndex(sf::Vector2i grid) const;
        
        // Check if both indexes have the same y coordinate.
        bool inSameRow(int index1, int index2) const;
        
        // Check if both indexes have the same x coordinate.
        bool inSameColumn(int index1, int index2) const;

        int panelSize() const;
        sf::Vector2f tileSize() const;
        sf::Vector2f tileOffset() const;
        int getPlayerQuantity() const;
        bool populationNeedsEnabled() const;
        bool buildingCostEnabled() const;
        bool astarEnabled() const;
        bool fogOfWarEnabled() const;
        float simulationSpeed() const;
        int getRegionPersistence() const;
        int getWorldMargin() const;
        int getWorldMarginPolar() const;
        float getWorldTerrainMin() const;
        int getWorldRiverQuantity() const;
        float getWorldForestMin() const;
        float getRegionTreeMin() const;
};
}

extern iso::GameSettings game_settings;