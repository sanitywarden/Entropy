#pragma once

#include "building.hpp"

#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <string>
#include <vector>

namespace lua {
namespace driver {
class Driver {
    private:
        // Lua wrappers.

        std::vector <iso::BuildingHarvest>    getHarvestedResourceList(const std::string& filename) const;
        std::vector <iso::BuildingProduction> getProducedResourceList(const std::string& filename) const;
        
        std::vector <std::string> getRegionRequirements(const std::string& filename) const;
        std::vector <std::string> getTileRequirements(const std::string& filename) const;

        std::vector <std::string> readBiomeTiles(const std::string& filename) const;
        std::vector <std::string> readBiomeTrees(const std::string& filename) const;

        std::vector <std::string> readVectorString(luabridge::LuaRef reference) const; 
        bool           readBoolean(luabridge::LuaRef reference)  const;
        std::string    readString(luabridge::LuaRef reference)   const;
        int            readInteger(luabridge::LuaRef reference)  const;
        float          readNumber(luabridge::LuaRef reference)   const;
        core::Vector2i readVector2i(luabridge::LuaRef reference) const;
        core::Colour   readColour(luabridge::LuaRef reference)   const;

        // Expose C++ to Lua.          
        
        void registerLua() const;

    public:
        lua_State* L;
    public:
        Driver();
        ~Driver();

        void stackSize() const;
        void stackDump() const;
};
}
}

extern lua::driver::Driver lua_driver;