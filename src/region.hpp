#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "biome.hpp"
#include "tile.hpp"
#include "regionType.hpp"
#include "gameObject.hpp"
#include "player.hpp"
#include "building.hpp"
#include "generationSettings.hpp"
#include "pawn.hpp"

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
        
        protected:
            bool           _marked; 
            RiverDirection _direction;      
            float          height;
            float          moisture;
            float          temperature;
            float          latitude;

        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        public:
            Region();
            ~Region();
            
            RiverDirection riverDirection();
            bool           isOwned();
            void           addResource         (ResourceType resource, int quantity);
            void           addResources        (ResourceCollection resource);
            int            getResourceQuantity (ResourceType resource);
            bool           isBuildingAffordable(const Building& building) const;
            bool           isPositionValid     (const Building& building, const GenerationSettings& settings, int index) const;
            void           placeBuilding       (Building building, const GenerationSettings& settings, int index);
            bool           placeBuildingCheck  (Building building, const GenerationSettings& settings, int index);
            void           removeBuilding      (int index);
            void           removeBuildingCost  (const Building& building);

        public:
            RegionType regiontype;
            GameObject forest;
            GameObject river;

            Biome biome;

            bool visited;

            std::vector <Tile>                         map;
            std::map    <int, GameObject>              trees;
            std::map    <int, std::vector<GameObject>> sides;
            std::map    <int, Building>                buildings;
            std::vector <Pawn>                         population;

            Player*            owner;     // Pointer to the player that owns this region. If no player controls this region, it's a nullptr.
            ResourceCollection resources; // These are resources that are already owned by a player. They are not placed in a "global" stockpile, they exist inside the region.
    };  
}

#endif