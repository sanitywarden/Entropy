#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "biome.hpp"
#include "tile.hpp"
#include "regionType.hpp"
#include "gameObject.hpp"
#include "player.hpp"
#include "resource.hpp"
#include "item.hpp"
#include "./building/building.hpp"
#include "./unit/unit.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace iso {
    enum class RiverDirection {
        RIVER_HORIZONTAL,
        RIVER_VERTICAL,
        RIVER_NORTH_TO_EAST,
        RIVER_NORTH_TO_WEST,
        RIVER_SOUTH_TO_EAST,
        RIVER_SOUTH_TO_WEST, 
        RIVER_NONE,          // River does not exist.
        RIVER_ORIGIN         // Beginning of the river, first tile.
    };

    class Region : public GameObject {
        friend class WorldGenerator;
        friend class SimulationManager;

        protected:
            bool           _marked; 
            RiverDirection _direction;      
            float          height;
            float          moisture;
            float          temperature;
            float          latitude;

        public:
            Region();
            ~Region();
            
            RiverDirection riverDirection();
            bool isOwned() const;
            void addItem(StorageItem item);
            int  getItemQuantity(StorageItem item) const;
            bool checkItemExists(StorageItem item) const;

            int  getFoodQuantity() const;
            int  getDrinkableLiquidQuantity() const;
            bool isBuildingAffordable(const Building& building) const;
            bool isPositionValid(const Building& building, sf::Vector2i grid_position) const;
            void placeBuilding(Building building, sf::Vector2f texture_size, sf::Vector2i grid_position);
            bool placeBuildingCheck(Building building, sf::Vector2f texture_size, sf::Vector2i grid_position);
            void removeBuilding(int index);
            void removeBuildingCost(const Building& building);
            int  getPopulation() const;

            bool isUnitPresent()   const;
            bool isTree(int index) const;
            bool isPath(int index) const;
            bool isSpotOccupied(int index) const;
            bool isSpotOccupied(sf::Vector2i grid_position) const;
            bool isPassableAStar(int index) const;

            // If building exists at provided index, return pointer.
            // Else returns nullptr.
            Building* getBuildingAt(int index) const;
            int isBuildingInProximity(const Building& building, int building_index) const;
            bool isSameBuilding(const Building& building, int building_index, int index) const;
            int findNotOccupiedTile(std::vector <int> buffer) const;

        public:
            RegionType regiontype;
            Biome      biome;
            Unit*      unit;
            Player*    owner; // Pointer to the player that owns this region. If no player controls this region, it's a nullptr.

            bool visited;
            std::string settlement_name; // Settlement's human readable name. If there is no settlement, it's "*".

            std::vector <Tile>                           map;
            std::vector <Unit>                           population;
            std::vector <StorageItem>                    storage;  // These are resources that are already owned by a player. They are not placed in a "global" stockpile, they exist inside the region.
            std::map    <int, GameObject>                trees;
            std::map    <int, std::vector<GameObject>>   sides;
            std::map    <int, std::shared_ptr<Building>> buildings;
    }; 
}

#endif
