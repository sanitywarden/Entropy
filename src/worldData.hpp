#pragma once

#include <SFML/System/Vector2.hpp>
#include <map>

#include "types.hpp"

namespace iso {    
struct WorldData {
    int w_persistence;
    int w_width;
    float w_terrain_from;
    float w_forest_from;
    float w_temperature_multiplier;
    float w_moisture_multiplier;
    int w_margin;
    int w_margin_polar;
    int w_river_quantity;

    int r_persistence;
    int r_width;
    float r_forest_from;
    
    core::Vector2i tile_size;

    float simulation_speed;
    bool building_cost_enabled;
    bool fog_of_war_enabled;

    WorldData();
    ~WorldData();
};

int getWorldSize();
bool inWorldBounds(int index);
bool inWorldBounds(core::Vector2i grid);
int calculateWorldIndex(int x, int y);
int calculateWorldIndex(core::Vector2i grid);

int getRegionSize();
bool inRegionBounds(int index);
bool inRegionBounds(core::Vector2i grid);
int calculateRegionIndex(int x, int y);
int calculateRegionIndex(core::Vector2i grid);   

// Returns true if both indexes have the same y coordinate.
bool inSameRow(int index1, int index2);

// Returns true if both indexes have the same x coordinate.
bool inSameColumn(int index1, int index2);

core::Vector2i tileGridPosition(int index);
core::Vector2i tileGridPosition(core::Vector2f tile_position); 

core::Vector2i tileSize();

inline int panelSize()             { return 128; }
inline core::Vector2i tileOffset() { return core::Vector2i(100, 100); }
inline int getWorldMargin()        { return 3; }
inline int getWorldMarginPolar()   { return 3; }
}

extern iso::WorldData world_data;
