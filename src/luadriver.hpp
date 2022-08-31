#pragma once

#include "building.hpp"
#include "simulationManager.hpp"

#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <string>
#include <vector>
#include <memory>

namespace lua {
void loadTexture(const std::string& filename, const std::string& id, core::Vector2i position = core::Vector2i(), core::Vector2i size = core::Vector2i());

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

    protected:
        Driver();

        static std::shared_ptr <Driver> instance;

    public:
        lua_State* L;

        ~Driver();

        Driver(Driver& driver) = delete;
        void operator= (const Driver& driver) = delete;

        static Driver* getInstance();

        void loadGameData();
        void loadResources(iso::SimulationManager* manager, const std::string& filename);

        void stackSize() const;
        void stackDump() const;
};
}
}