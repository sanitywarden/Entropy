#pragma once

#include <string>
#include <vector>
#include <Lua/lua.hpp>

namespace lua {
class LuaScript { 
    private:
        std::string filename;

    public:
        LuaScript();
        LuaScript(const std::string& filename);
        ~LuaScript();

        void onEvent(lua_State* L, const std::string& event) const;
};
}

extern std::vector <lua::LuaScript> scripts;