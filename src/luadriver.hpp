#pragma once

#include <Lua/lua.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

#include "building.hpp"
#include "entropy/engineSettings.hpp"

namespace lua {
namespace driver {
class Driver {
    private:
        // Lua wrappers.

        std::string getFieldValueString(const std::string& field, const std::string& key) const;
        int getFieldValueInteger(const std::string& field, const std::string& key) const;
        float getFieldValueFloat(const std::string& field, const std::string& key) const;
        bool getFieldValueBoolean(const std::string& field, const std::string& key) const;
        sf::Vector2i getFieldValueVector2i(const std::string& field, const std::string& key1, const std::string& key2) const;
        
        std::string getNestedFieldValueString(const std::string& field1, const std::string& field2, const std::string& key) const;
        int getNestedFieldValueInteger(const std::string& field1, const std::string& field2, const std::string& key) const;
        float getNestedFieldValueFloat(const std::string& field1, const std::string& field2, const std::string& key) const;
        bool getNestedFieldValueBoolean(const std::string& field1, const std::string& field2, const std::string& key) const;
        sf::Vector2i getNestedFieldValueVector2i(const std::string& field1, const std::string& field2, const std::string& key1, const std::string& key2) const;
        
        std::vector <iso::BuildingHarvest>    getHarvestedResourceList(const std::string& filename) const;
        std::vector <iso::BuildingProduction> getProducedResourceList(const std::string& filename) const;


        // Definitions of lua exposed functions.                
        
    private:
        lua_State* L;
        iso::Settings app_settings;

    public:
        Driver();
        ~Driver();

        void stackSize() const;
        void stackDump() const;

        iso::Settings getApplicationSettings() const;
};
}
}

extern lua::driver::Driver lua_driver;