#include "luascript.hpp"

#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include "simulationManager.hpp"

using namespace lua;

LuaScript::LuaScript()
{}

LuaScript::LuaScript(const std::string& filename)
    : filename(filename)
{}

LuaScript::~LuaScript()
{}

void LuaScript::onEvent(const std::string& event) const {
    runLuaFile(this->filename);
    
    auto script_data = luabridge::getGlobal(game_manager.lua(), "Script");

    const auto& name = script_data["on_event"];
    if(name == event) {
        auto execute = script_data["execute"];
        
        if(!execute.isNil())
            execute();
    }
}