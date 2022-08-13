#include "luascript.hpp"

#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>

using namespace lua;

LuaScript::LuaScript()
{}

LuaScript::LuaScript(const std::string& filename)
    : filename(filename)
{}

LuaScript::~LuaScript()
{}

void LuaScript::onEvent(lua_State* L, const std::string& event) const {
    luaL_dofile(L, this->filename.c_str());
    auto function = luabridge::getGlobal(L, "onEvent");
    
    if(!function.isNil())
        function(event);
}