#pragma once

#include "types.hpp"
#include "gameObject.hpp"
#include "resource.hpp"
#include "item.hpp"

namespace iso {
typedef std::vector <StorageItem> ResourceList;

// Description of a item produced by a building.
struct BuildingProduction {
    std::string       name;         // Name of the produced item.
    int               amount;       // Amount of the produced item.
    iso::ResourceList requirements; // Resources required to produce amount of items every building update.
};

// Description of a resource harvest by a building. 
struct BuildingHarvest {
    std::string       name;    // Name of the harvested resource.
    iso::ResourceList harvest; // Amount of resources harvested every building update.
};

struct BuildingData {
    std::string          filename;     // Path to the definition file of the building.
    std::string          name;         // Human readable name.
    std::string          description;  // Human readable description.
    core::Vector2i       size;         // Building size in tiles.
    std::string          texture;      // Default building texture. The texture rendered is GameObject::object_texture_name.
    core::Vector2i       texture_size; // Texture size in pixels.
    std::string          icon_texture; // Icon texture.
    core::Vector2i       icon_size;    // Icon size in pixels.
    bool                 removable;    // Is the building destroyable.
    iso::ResourceList    cost;         // Resource cost of this building.
    iso::ResourceList    refund;       // Resource refund granted upon removal.
    core::Vector2i       scan_area;    // In what area does this building look for resources.
    std::vector <BuildingHarvest>    harvests; // What does this building harvest.
    std::vector <BuildingProduction> produces; // What does this building produce.
};

class Building : public GameObject {
    protected:
        BuildingData data;
    
    private:
        void harvest(GameObject* region, int index) const;
        void produce(GameObject* region, int index) const;

    public:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        
        Building();
        Building(const Building& building);
        Building(const BuildingData& data);
        ~Building();
        
        const std::string&    getDefinitionFilename()  const;
        const std::string&    getBuildingName()        const;
        const std::string&    getBuildingDescription() const;
        core::Vector2i        getBuildingArea()        const;
        const std::string&    getBuildingTexture()     const;
        core::Vector2i        getBuildingScanArea()    const;
        const ResourceList&   getBuildingCost()        const;
        const ResourceList&   getBuildingRefund()      const;
        const std::string&    getBuildingIcon()        const;
        const core::Vector2i  getBuildingIconSize()    const;        
        const std::vector <BuildingHarvest>&    getBuildingHarvests()   const;
        const std::vector <BuildingProduction>& getBuildingProduction() const;
        bool isProductionBuilding() const;
        bool isHarvestBuilding()    const;
        bool isRemovable()          const;
        bool isTileHarvestable(int region_index, int tile_index) const;

        void setBuildingName(const std::string& name);
        void setBuildingDescription(const std::string& description);
        void setBuildingArea(core::Vector2i area);
        void setBuildingTexture(const std::string& texture_name);
        void setBuildingScanArea(core::Vector2i scan_area);
        void setBuildingCost(ResourceList list);
        void setBuildingIcon(const std::string& icon);
        void setBuildingIconSize(core::Vector2i size);
        void setBuildingHarvests(std::vector <BuildingHarvest> harvest);
        void setBuildingProduction(std::vector <BuildingProduction> production);
        void setRemovable(bool removable);

        bool operator == (const Building& building) const;
        bool operator != (const Building& building) const;

};

const Building BUILDING_EMPTY;
}

extern std::vector <iso::Building> BUILDING_TABLE;