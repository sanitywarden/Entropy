#pragma once

#include "types.hpp"

#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <vector>
#include <string>

namespace gui {    
class AbstractWidget;
typedef std::shared_ptr <AbstractWidget> AbstractComponent; 

enum class TriggerType {
    WIDGET_SELECTED, 
    WIDGET_ALL      
};

static std::map <std::string, TriggerType> GUI_EVENT_LIST = {
    { "update",             TriggerType::WIDGET_ALL },
    { "render",             TriggerType::WIDGET_ALL },
    { "onVisibilityToggle", TriggerType::WIDGET_ALL },
    { "onShow",             TriggerType::WIDGET_ALL },
    { "onHide",             TriggerType::WIDGET_ALL },

    { "onMouseButtonPress",       TriggerType::WIDGET_SELECTED },
    { "onMouseButtonRelease",     TriggerType::WIDGET_SELECTED },
    { "onLeftMouseButtonPress",   TriggerType::WIDGET_SELECTED },
    { "onRightMouseButtonPress",  TriggerType::WIDGET_SELECTED },
    { "onMiddleMouseButtonPress", TriggerType::WIDGET_SELECTED },
    { "onKeyPress",               TriggerType::WIDGET_SELECTED },
    { "onKeyRelease",             TriggerType::WIDGET_SELECTED },
    { "onScroll",                 TriggerType::WIDGET_SELECTED },
    { "onScrollUp",               TriggerType::WIDGET_SELECTED },
    { "onScrollDown",             TriggerType::WIDGET_SELECTED },
    { "onMouseMove",              TriggerType::WIDGET_SELECTED },
};

struct WidgetData {
    std::string widget_id;
    core::Vector2i position;
    core::Vector2i size;
    AbstractComponent parent;
    int draw_priority; // Drawing priority. Pages with higher number will be drawn over pages with lower number.
    bool draw;

    std::map <std::string, AbstractComponent> components;
    std::vector <std::string> event_overrides;
};

class AbstractWidget : public sf::Drawable {
    friend class InterfacePage;

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { return; };

    protected:
        WidgetData data;

    public:
        AbstractWidget();
        AbstractWidget(const WidgetData& data);
        ~AbstractWidget();

        void setWidgetID(const std::string& id);
        const std::string& getWidgetID() const;

        void setWidgetPosition(core::Vector2i position);
        void setWidgetPosition(int x, int y);
        core::Vector2i getWidgetPosition() const;
        
        void setWidgetSize(core::Vector2i size);
        void setWidgetSize(int x, int y);
        core::Vector2i getWidgetSize() const;

        void setParent(AbstractComponent parent);
        AbstractWidget* const getParent() const;
        bool hasParent() const;


        void setVisible(bool visible);
        bool isVisible() const;
        void toggleVisibile();

        void setDrawPriority(int priority);
        int getDrawPriority() const;

        bool containsPoint(core::Vector2i point) const;

        void addEventOverride(const std::string& event_name);
        bool hasEventOverride(const std::string& event_name) const;

        void addComponent(AbstractComponent component);
        void removeComponent(const std::string& id);
        bool hasChildren() const;

        const WidgetData& getWidgetData() const;
};
}