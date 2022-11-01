#pragma once

#include <string>
#include <vector>
#include <map>

namespace lua {
static std::vector <std::string> SCRIPTABLE_OBJECT_EVENTS = {
    "onMouseButtonPress", 
    "onMouseButtonRelease", 
    "onLeftMouseButtonPress", 
    "onRightMouseButtonPress", 
    "onMiddleMouseButtonPress", 
    "onKeyPress", 
    "onKeyRelease", 
    "onScroll", 
    "onScrollUp", 
    "onScrollDown", 
    "onMouseMove", 
    "onWindowResize", 
    "onBuildingConstruct", 
    "onBuildingDestroy", 
};

struct ScriptData {
    std::string filename;
    std::vector <std::string> event_overrides;
};

class ScriptableObject { 
    protected:
        ScriptData data;

    public:
        ScriptableObject(const ScriptData& data);
        ~ScriptableObject();

        bool hasEventOverride(const std::string& event_name) const;
        void handleEvent(const std::string& event_name) const; 
};
}

extern std::vector <lua::ScriptableObject> SCRIPT_TABLE;