#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "resources.hpp"
#include "./unit/unit.hpp"

#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <memory>

namespace iso {
    class Player {
        friend class SimulationManager;

        public:
            std::vector <std::shared_ptr <Unit>> units;
            std::vector <int> owned_regions;  // Indexes of the player owned regions. 
            int               capital_region; // Index of the capital city.
            bool              is_human;
            sf::Color         team_colour;
            std::string       country_name;
            int               player_id;      // Used to differenciate between players.

        public:
            Player();
            ~Player();

            void                      addOwnedRegion(int region_index);
            void                      removeOwnedRegion(int region_index);
            const std::vector <int>&  readOwnedRegions();
            int                       empireSize() const; // Number of regions that the player controls.
            int                       getCapital() const;
            bool                      hasCapital() const;
            void                      setCapital(int region_index);
            void                      setHuman(bool is_human);
            bool                      isHuman() const;
            const sf::Color           getTeamColourFull() const;
            const sf::Color           getTeamColourTransparent() const;
            void                      setTeamColour(const sf::Color& team_colour);
            void                      setCountryName(const std::string& name);
            const std::string&        getCountryName();
            void                      addUnit(std::shared_ptr <Unit> unit);
            Unit*                     getUnit(int unit_id);
            Unit*                     getUnit(std::string unit_name);
            bool                      hasUnit(int unit_id) const;
            void                      removeUnit(int unit_id);
            int                       getID() const;
    };
}

#endif