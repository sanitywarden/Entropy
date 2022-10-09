#pragma once

#include "building.hpp"
#include "region.hpp"
#include "types.hpp"

namespace iso {
struct PlayerData {
    int id;            // Numerial id of a country and a player.
    bool is_human;
    std::string username;
};

struct CountryData {
    int id;             // Numerical id of a country and a player.
    int capital;        // Region index of the capital city. If does not have capital, it is set to -1.
    int initial_spawn;  // Region index the player will spawn in first time he loads the game.
    std::vector <int> owned_regions; // Indexes of the country owned regions.
    std::vector <int> known_regions; // Indexes of the country discovered regions.

    std::string  country_name;
    core::Colour map_colour;
};

class Player {
    friend class SimulationManager;

    protected:
        PlayerData  player_data;
        CountryData country_data;

    public:
        Player(PlayerData pdata, CountryData cdata);
        ~Player();

        bool canAffordBuilding(const Region& region, const Building& building) const;
        bool placeBuildingCheck(Region& region, Building building, core::Vector2i grid) const;
        void placeBuilding(Region& region, Building building, core::Vector2i grid) const;
        void destroyBuilding(Region& region, core::Vector2i grid) const;
        void addOwnedRegion(int index);
        void removeOwnedRegion(int index);
        bool ownsRegion(int index) const;
        Region& getOwnedRegion(int index);
        void addKnownRegion(int index);
        bool discoveredRegion(int index) const;
        int  countrySize() const;
        int  getCapital() const;
        bool hasCapital() const;
        void setCapital(int index);
        bool isHuman() const;
        void setHuman(bool is_human);
        core::Colour getCountryColourFull() const;
        core::Colour getCountryColourTransparent() const;
        void setCountryColour(core::Colour team_colour);
        void setCountryName(const std::string& name);
        const std::string& getCountryName();
        int getID() const;
};
}