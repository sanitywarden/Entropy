#include "luascript.hpp"
#include "simulationManager.hpp"

#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>


namespace lua {
ScriptableObject::ScriptableObject(const ScriptData& data)
    : data(data)
{}

ScriptableObject::~ScriptableObject()
{}

bool ScriptableObject::hasEventOverride(const std::string& event_name) const {
    return std::find(this->data.event_overrides.begin(), this->data.event_overrides.end(), event_name) != this->data.event_overrides.end();
}

void ScriptableObject::handleEvent(const std::string& event_name) const {
    if(!this->hasEventOverride(event_name))
        return;
    
    runLuaFile(this->data.filename);
    auto script = luabridge::getGlobal(game_manager.lua(), "Script");
    auto event_override = script[event_name];
    event_override();
}
}