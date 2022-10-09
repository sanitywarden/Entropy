#pragma once

#include "types.hpp"

#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

namespace gui {    
class AbstractWidget;
typedef std::shared_ptr <AbstractWidget> AbstractComponent; 

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
};
}