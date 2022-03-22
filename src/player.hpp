#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "resources.hpp"
#include "./unit/unit.hpp"

#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>

namespace iso {
    class Player {
        friend class SimulationManager;

        protected:
            std::vector <int>  owned_regions; // Indexes of the player owned regions. 
            bool               is_human;
            sf::Color          team_colour;
            std::string        country_name;
            std::vector <Unit> units;

        public:
            Player();
            ~Player();

            void                      addOwnedRegion(int region_index);
            void                      removeOwnedRegion(int region_index);
            const std::vector <int>&  readOwnedRegions();
            int                       getCapital();
            void                      setHuman(bool is_human);
            bool                      isHuman() const;
            const sf::Color&          getTeamColour();
            void                      setCountryName(std::string name);
            const std::string&        getCountryName();
            void                      addUnit(Unit unit);
            Unit*                     getUnit(int unit_id);
            const std::vector <Unit>& seeUnits() const;
            std::vector <Unit>&       getUnits();
    };
}

#endif