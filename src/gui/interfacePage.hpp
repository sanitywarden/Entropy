#pragma once

#include "gui/abstractWidget.hpp"
#include "gui/widget.hpp"
#include "gui/button.hpp"
#include "gui/label.hpp"

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <string>
#include <memory>

namespace gui {
/*
typedef std::shared_ptr <ImageHolder>    ImageComponent;
typedef std::shared_ptr <InputBox>       InputBoxComponent;
*/

enum class GUIObject {
    WIDGET,
    BUTTON,
    BUTTON_LIST,
    LABEL,
    IMAGE,
    IMAGE_LIST,
    INPUT_BOX
};

static std::map <std::string, GUIObject> WIDGET_ID_TABLE = {
    { "Widget"    , GUIObject::WIDGET      },
    { "Button"    , GUIObject::BUTTON      },
    { "ButtonList", GUIObject::BUTTON_LIST },
    { "Label"     , GUIObject::LABEL       },
    { "Image"     , GUIObject::IMAGE       },
    { "ImageList" , GUIObject::IMAGE_LIST  },
    { "InputBox"  , GUIObject::INPUT_BOX   },
};

static std::vector <std::string> GUI_EVENT_LIST = {
    "onMouseButtonPress",
    "onMouseButtonRelease",
    "onLeftMouseButtonPress",
    "onRightMouseButtonPress",
    "onMiddleMouseButtonPress",
    "onKeyPress",
    "onKeyRelease",
    "onScroll",
    "onScrollUp",
    "onScrollDown"
};

class InterfacePage : public AbstractWidget {
    private:
        InterfacePage() = delete;
    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    public:
        std::string definition_filename;
        std::string functionality_filename;
        bool closable;       
        bool movable;
        bool resizable;
        std::vector <AbstractComponent> interface;
    public:
        InterfacePage(const std::string& definition_filename, const std::string& functionality_filename);
        ~InterfacePage();

        void createUI();

        void handleGUIEvent(const std::string& event_name) const;
        bool intersectsUI(core::Vector2i point) const;
        void addComponent(AbstractComponent component);
        void deleteComponent(const std::string& id);
        AbstractComponent getComponent(const std::string& id) const;
        AbstractComponent getCurrentComponent() const;
};

typedef std::shared_ptr <InterfacePage> InterfaceComponent;
}