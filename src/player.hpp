#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "region.hpp"
#include "building.hpp"
#include "resourceCost.hpp"

#include <vector>

namespace iso {
    const sf::Color COLOUR_RED    (229, 20, 0, 255);
    const sf::Color COLOUR_BLUE   (0, 80, 239, 255);
    const sf::Color COLOUR_GREEN  (0, 138, 0, 255);
    const sf::Color COLOUR_PURPLE (106, 0, 255, 255);
    const sf::Color COLOUR_YELLOW (227, 200, 0, 255);
    const sf::Color COLOUR_CYAN   (27, 161, 226, 255);
    const sf::Color COLOUR_WHITE  (255, 255, 255, 255);
    const sf::Color COLOUR_BLACK  (0, 0, 0, 255);
    
    const std::vector <sf::Color> team_colours = {
        COLOUR_RED, 
        COLOUR_BLUE, 
        COLOUR_GREEN,
        COLOUR_PURPLE,
        COLOUR_YELLOW,
        COLOUR_CYAN
    };

    class Player {
        friend class SimulationManager;
        friend class BuildingManager;

        private:

        protected:
            ResourceCost      resources;
            std::vector <int> owned_regions; // Indexes of the player owned regions. 
            bool              is_human;
            sf::Color         team_colour;

        public:
            Player();
            ~Player();

            void addOwnedRegion(int region_index);
            void removeOwnedRegion(int region_index);
            void setHuman(bool is_human);
            bool isHuman();
            void addResource(Resource resource, int quantity);
            void addResources(ResourceCost resource);
            int  getResourceQuantity(Resource resource);
            
            bool isBuildingAffordable(const Building building);
            void removeBuildingCost(const Building building);
    };
}

#endif