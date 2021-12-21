#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "region.hpp"

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

    class PlayerResources {
        public:
            PlayerResources();
            ~PlayerResources();

            std::vector <int> owned_regions; // Indexes of the player owned regions. 
            int gold;
            int wood;
            int food;
    };

    class Player {
        friend class SimulationManager;

        private:

        protected:
            PlayerResources resources;

            bool player_is_human;
            sf::Color player_colour;

        public:
            Player();
            ~Player();

            void addOwnedRegion(int region_index);
            void removeOwnedRegion(int region_index);
            void setHuman(bool player_is_human);
            bool isHuman();
    };
}

#endif