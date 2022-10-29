#pragma once

#include "gameObject.hpp"
#include "regionType.hpp"
#include "biome.hpp"
#include "tile.hpp"
#include "resource.hpp"
#include "building.hpp"
#include "item.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

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

    public:
        bool           _marked; 
        RiverDirection _direction;      
        float          height;
        float          latitude;

    public:
        Region();
        ~Region();
        
        RiverDirection riverDirection();
        
        bool isBuildingPositionValid(const Building& building, core::Vector2i grid) const;
        void removeBuildingCost(const Building& building);

        bool isOwned() const;
        int  getOwnerId() const;

        bool isPassableWorldmap() const;
        bool isSpotOccupied(int index) const;
        bool isSpotOccupied(core::Vector2i grid) const;
        bool isPassableRegionmap(int index) const;
        bool isPassableRegionmap(core::Vector2i) const;

        void stockpileAdd(StorageItem item);
        void stockpileRemove(StorageItem item);
        int  itemQuantity(const StorageItem& item) const;
        bool itemExists(const StorageItem& item) const; 

        bool resourceExistsAt(int index) const;
        bool resourceExistsAt(core::Vector2i grid) const;
        const Resource& getResourceAt(int index) const;
        const Resource& getResourceAt(core::Vector2i grid) const;

        bool treeExistsAt(int index) const;
        bool treeExistsAt(core::Vector2i grid) const;

        bool buildingExistsAtIndex(int index) const;
        bool buildingExistsAtGrid(core::Vector2i grid) const;
        const Building& getBuildingAtIndex(int index) const;
        const Building& getBuildingAtGrid(core::Vector2i grid) const;

        Tile& getTileAtIndex(int index);
        bool isBiome(const Biome& biome) const;

        bool L_hasBiome() const;
        Biome* L_getBiome();
        bool L_isTerrain() const;
        bool L_isOcean() const;
        bool L_isRiver() const;
        bool L_isCoast() const;
        bool L_isLake() const;
        bool L_isWater() const;
        bool L_isForest() const;
        Tile* L_getTileAt(int index);
        bool L_isTileOccupied(int index) const;
        bool L_isBuildingPositionValid(const Building& building, int index) const;
        bool L_treeExistsAt(int index) const;
        GameObject* L_getTreeAt(int index);
        bool L_buildingExistsAt(int index) const;
        Building* L_getBuildingAt(int index);
        bool L_resourceExistsAt(int index) const;
        Resource* L_getResourceAt(int index);
        const std::string& L_getMoistureString() const;
        float L_getMoistureNumber() const;
        const std::string& L_getTemperatureString() const;
        float L_getTemperatureNumber() const;
        void L_constructBuilding(Building* building, int tile_index);
        void L_demolishBuilding(int tile_index);         
        bool L_hasOwner() const;
        int  L_getOwnerId() const;

    public:
        RegionType regiontype;
        Biome      biome;

        int owner_id;

        float moisture;
        std::string moisture_text;

        float temperature;
        std::string temperature_text;

        bool visited;
        std::string settlement_name; // Settlement's human readable name. If there is no settlement, it's "*".

        std::vector <Tile> map;

        // These are resources that are already owned by a player. 
        // They are not placed in a global stockpile, they are stored in the region.
        // Who controls the region, controls the stockpile.
        std::vector <StorageItem> stockpile; 
        std::map <int, Building>  buildings;
        std::map <int, Resource>  resources;
        // std::vector <Unit> worldmap_units;   // Worldmap units that station in the region. 
        // std::vector <Unit> regionmap_units;  // Regionmap units.
        
        std::map <int, GameObject>              trees;
        std::map <int, std::vector<GameObject>> sides;
}; 
}
