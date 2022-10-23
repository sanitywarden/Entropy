#pragma once

#include <string>
#include <vector>
#include <Lua/lua.hpp>

namespace lua {
class LuaScript { 
    public:
        std::string filename;

    public:
        LuaScript();
        LuaScript(const std::string& filename);
        ~LuaScript();

        void onEvent(const std::string& event) const;
};
}

extern std::vector <lua::LuaScript> SCRIPT_TABLE;