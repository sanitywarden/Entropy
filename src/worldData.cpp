#include "worldData.hpp"
#include "globalutilities.hpp"
#include "colours.hpp"
#include "simulationManager.hpp"

namespace iso {
int getWorldSize() { 
    return world_data.w_width * world_data.w_width;
}

bool inWorldBounds(int index) {
    return (index >= 0 && index < getWorldSize());
}

bool inWorldBounds(core::Vector2i grid) {
    return (grid.x >= 0 && grid.y >= 0 && grid.x < world_data.w_width && grid.y < world_data.w_width);
}

int calculateWorldIndex(int x, int y) {
    return y * world_data.w_width + x;
}

int calculateWorldIndex(core::Vector2i grid) {
    return calculateWorldIndex(grid.x, grid.y);
}

int getRegionSize()  {
    return world_data.r_width * world_data.r_width;
}

bool inRegionBounds(int index)  {
    return (index >= 0 && index < getRegionSize());
}

int calculateRegionIndex(int x, int y)  {
    return y * world_data.r_width + x;
}

int calculateRegionIndex(core::Vector2i grid)  {
    return calculateRegionIndex(grid.x, grid.y);
}

bool inRegionBounds(core::Vector2i grid_position)  {
    return (
        grid_position.x >= 0 && grid_position.x < world_data.r_width && 
        grid_position.y >= 0 && grid_position.y < world_data.r_width
    );
}

bool inSameRow(int index1, int index2)  {
    auto grid1 = tileGridPosition(index1);
    auto grid2 = tileGridPosition(index2);

    return grid1.y == grid2.y; 
}

bool inSameColumn(int index1, int index2)  {
    auto grid1 = tileGridPosition(index1);
    auto grid2 = tileGridPosition(index2);

    return grid1.x == grid2.x; 
}

core::Vector2i tileGridPosition(int index) {
    return core::Vector2i(
        index % world_data.r_width,
        index / world_data.r_width
    );
}

core::Vector2i tileGridPosition(core::Vector2f tile_position) {
    auto cell = core::Vector2i(
        tile_position.x / tileSize().x,
        tile_position.y / tileSize().y
    );

    auto selected = core::Vector2i(
        (cell.y - tileOffset().y) + (cell.x - tileOffset().x),
        (cell.y - tileOffset().y) - (cell.x - tileOffset().x)
    );

    auto position_within_tile = core::Vector2i(
        (int)tile_position.x % (int)tileSize().x,
        (int)tile_position.y % (int)tileSize().y
    );

    auto colour = getTilePixelColour(position_within_tile);
    if(colour == COLOUR_RED)
        selected += core::Vector2i(-1, 0);
    else if(colour == COLOUR_GREEN)
        selected += core::Vector2i(1, 0);
    else if(colour == COLOUR_BLUE)
        selected += core::Vector2i(0, -1);
    else if(colour == COLOUR_YELLOW)
        selected += core::Vector2i(1, 0);
    
    return selected;
}
}