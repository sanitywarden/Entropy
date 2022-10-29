#include "interfacePage.hpp"
#include "simulationManager.hpp"
#include "globalutilities.hpp"
#include "guiutilities.hpp"
#include "gamestate.hpp"

#include <iostream>

namespace gui {
InterfacePage::InterfacePage(const std::string& definition_filename, const std::string& functionality_filename)  
    : definition_filename(definition_filename), functionality_filename(functionality_filename)
{
    this->createUI();   
}

InterfacePage::~InterfacePage() 
{}

void InterfacePage::createUI() {
    // Interpret GUI definition file.

    {
        lua::runLuaFile(this->definition_filename);

        auto definition = luabridge::getGlobal(game_manager.lua(), "UI");

        // Read data about the main component.

        this->data.widget_id = lua::readString(definition["id"], true);
        this->closable  = lua::readBoolean(definition["closeable"], true);
        this->movable   = lua::readBoolean(definition["movable"], true);
        this->resizable = lua::readBoolean(definition["resizable"], true);

        auto components = definition["components"];
        auto list_length = components.length();

        // Read data from the component list.
        
        for(int i = 1; i <= list_length; i++) {
            auto table = components[i];
            auto type = lua::readString(table["component_type"], true); 

            if(!WIDGET_ID_TABLE.count(type)) {
                iso::printError("InterfacePage::InterfacePage()", "Could not find id of GUI Object '" + type + "'");
                return;
            }

            auto parent_id    = lua::readString(table["parent_id"], true);
            auto id           = lua::readString(table["id"], true);
            auto texture_base = lua::readString(table["texture"]); 
            auto position_tl_ratio = lua::readScreenRatio(table["position"]["corner_tl"], true);
            auto position_br_ratio = lua::readScreenRatio(table["position"]["corner_br"], true);

            auto object_id = WIDGET_ID_TABLE.at(type);
            switch(object_id) {
                default: {
                    iso::printError("InterfacePage::createUI()", "GUI Object type not implemented (GUIObject::" + std::to_string((int)object_id) + ")");
                    break;
                }

                case GUIObject::WIDGET: {
                    auto block_size = game_manager.resource.getTextureSize(texture_base + "_single").x;

                    WidgetData data;
                    data.parent        = this->getComponent(parent_id);
                    data.draw_priority = 1;
                    data.draw          = true;
                    data.widget_id     = id;

                    auto position_tl = calculateRelativeWidgetPosition(position_tl_ratio, data.parent);
                    auto position_br = calculateRelativeWidgetPosition(position_br_ratio, data.parent);
                    data.position    = position_tl;

                    auto widget_dimensions = calculateTileableDimensions(position_tl, position_br, block_size); 
                    data.size = calculateSizeFromDimensions(widget_dimensions, block_size);

                    auto widget = WidgetComponent(new Widget(data, widget_dimensions, texture_base));
                    this->addComponent(widget);

                    break;
                };

                case GUIObject::BUTTON: {
                    int draw_priority = this->getComponent(parent_id) == nullptr
                        ? 1
                        : this->getComponent(parent_id).get()->getDrawPriority() + 1;

                    auto block_size = game_manager.resource.getTextureSize(texture_base + "_single").x;

                    WidgetData data;
                    data.parent        = this->getComponent(parent_id);
                    data.draw_priority = draw_priority;
                    data.draw          = true; 
                    data.widget_id     = id;

                    auto position_tl = calculateRelativeWidgetPosition(position_tl_ratio, data.parent);
                    auto position_br = calculateRelativeWidgetPosition(position_br_ratio, data.parent);
                    data.position    = position_tl;

                    auto widget_dimensions = calculateTileableDimensions(position_tl, position_br, block_size); 
                    data.size = calculateSizeFromDimensions(widget_dimensions, block_size);

                    auto button = ButtonComponent(new Button(data, widget_dimensions, texture_base));
                    this->addComponent(button);

                    auto text = lua::readString(table["text"], true);

                    WidgetData label_data;
                    label_data.parent        = this->getComponent(id);
                    label_data.draw_priority = label_data.parent.get()->getDrawPriority() + 1;
                    label_data.draw          = true;
                    label_data.widget_id     = id + "_label"; 

                    button.get()->addLabel(label_data, text);
                    break;
                }
            
                case GUIObject::LABEL: {
                    int draw_priority = this->getComponent(parent_id) == nullptr
                        ? 1
                        : this->getComponent(parent_id).get()->getDrawPriority() + 1;
                    
                    WidgetData data;
                    data.parent        = this->getComponent(parent_id);
                    data.draw_priority = draw_priority;      
                    data.draw          = true;
                    data.widget_id     = id;

                    auto position = calculateRelativeWidgetPosition(position_tl_ratio, data.parent);
                    data.position = position;

                    auto alignment = table["alignment"];

                    auto font   = lua::readString(table["font"], true);
                    auto colour = lua::readColour(table["colour"], true);
                    auto alignment_x_text = lua::readString(alignment["x"], true);
                    auto alignment_y_text = lua::readString(alignment["y"], true);

                    if(!game_manager.resource.checkFontExists(font))
                        iso::printError("InterfacePage::createUI()", "Invalid font name '" + font + "'");

                    if(!ALIGNMENT_ID_TABLE.count(alignment_x_text))
                        iso::printError("InterfacePage::createUI()", "Invalid alignment '" + alignment_x_text + "'");

                    if(!ALIGNMENT_ID_TABLE.count(alignment_y_text))
                        iso::printError("InterfacePage::createUI()", "Invalid alignment '" + alignment_y_text + "'");                    

                    auto alignment_x = ALIGNMENT_ID_TABLE.at(alignment_x_text);
                    auto alignment_y = ALIGNMENT_ID_TABLE.at(alignment_y_text);   
    
                    auto label = LabelComponent(new Label(data, "", alignment_x, alignment_y));
                    
                    label.get()->setColour(colour);
                    label.get()->setFont(font);
                    
                    this->addComponent(label);
                    break;
                }
            
                case GUIObject::IMAGE: {
                    int draw_priority = this->getComponent(parent_id) == nullptr
                        ? 1
                        : this->getComponent(parent_id).get()->getDrawPriority() + 1;

                    WidgetData data;
                    data.parent        = this->getComponent(parent_id);
                    data.draw_priority = draw_priority;
                    data.draw          = true;
                    data.widget_id     = id;

                    auto position = calculateRelativeWidgetPosition(position_tl_ratio, data.parent);
                    data.position = position;

                    auto image = ImageComponent(new Image(data, texture_base));
                    this->addComponent(image);
                    break;
                }

                case GUIObject::IMAGE_LIST: {
                    int draw_priority = this->getComponent(parent_id) == nullptr
                        ? 1
                        : this->getComponent(parent_id).get()->getDrawPriority() + 1;

                    WidgetData data;
                    data.parent = this->getComponent(parent_id);
                    data.draw_priority = draw_priority;
                    data.draw = true;
                    data.widget_id = id;

                    if(data.parent == nullptr) {
                        iso::printError("InterfacePage::createUI()", "ImageList must have a parent");
                        return;
                    }

                    auto position = calculateRelativeWidgetPosition(position_tl_ratio, data.parent);
                    data.position = position;

                    auto size_ratio = position_br_ratio - position_tl_ratio;
                    auto size = core::Vector2i(
                        size_ratio.x * data.parent.get()->getWidgetSize().x,
                        size_ratio.y * data.parent.get()->getWidgetSize().y
                    );

                    data.size = size;

                    auto corner_ratio = lua::readScreenRatio(table["separator"]["corner"]);
                    auto image_ratio  = lua::readScreenRatio(table["separator"]["image"]);

                    auto corner = core::Vector2f(corner_ratio.x, corner_ratio.y);
                    auto image  = core::Vector2f(image_ratio.x, image_ratio.y);

                    auto image_list = ImageListComponent(new ImageList(data, corner, image));
                    this->addComponent(image_list);
                    break;
                }
            }
        }   
    }

    // Interpret GUI functionality file.

    {
        lua::runLuaFile(this->functionality_filename);

        auto definition = luabridge::getGlobal(game_manager.lua(), "Functionality");

        auto components = definition["components"];
        auto length = components.length();

        for(int i = 1; i <= length; i++) {
            auto table = components[i];
            auto id = lua::readString(table["id"], true);
            auto component = this->getComponent(id);
            
            for(const auto& pair : GUI_EVENT_LIST) {
                auto event_name = pair.first;
                auto event_reference = table[event_name];

                if(!event_reference.isNil() && !component.get()->hasEventOverride(event_name))
                    component.get()->addEventOverride(event_name);
            }
        }
    }

    // Sort the UI components by priority for rendering.

    {
        auto compare = [](AbstractComponent component1, AbstractComponent component2) {
            return component1.get()->getDrawPriority() < component2.get()->getDrawPriority();
        };

        std::sort(this->interface.begin(), this->interface.end(), compare);        
    }
}

AbstractComponent InterfacePage::getComponent(const std::string& id) const {
    for(auto it = this->interface.begin(); it != this->interface.end(); ++it) {
        if((*it).get()->getWidgetID() == id)
            return *it; 
    }

    return nullptr;
}

void InterfacePage::addComponent(AbstractComponent component) {
    if(component == nullptr)
        iso::printError("InterfacePage::addComponent()", "Component is a nullptr");

    this->interface.push_back(component);
}

void InterfacePage::deleteComponent(const std::string& id) {
    for(auto it = this->interface.begin(); it != this->interface.end(); ++it) {
        if((*it).get()->getWidgetID() == id)
            this->interface.erase(it);
    }
}

bool InterfacePage::intersectsUI(core::Vector2i point) const {
    for(const auto& component : this->interface) {
        if(component.get()->containsPoint(point))
            return true;
    }

    return false;
}

void InterfacePage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(const auto& component : this->interface) {
        if(component.get()->isVisible())
            target.draw(*component);
    }
}

void InterfacePage::handleGUIEvent(const std::string& event_name) const {
    if(!GUI_EVENT_LIST.count(event_name))
        iso::printError("InterfacePage::handleGUIEvent()", "No event with ID '" + event_name + "'");

    lua::runLuaFile(this->functionality_filename);                
    auto functionality = luabridge::getGlobal(game_manager.lua(), "Functionality");
    auto components = functionality["components"];
    auto length = components.length();

    auto trigger_type = GUI_EVENT_LIST.at(event_name);
    switch(trigger_type) {
        default: {
            iso::printError("InterfacePage::handleGUIEvent()", "Trigger type not implemented (TriggerType::" + std::to_string((int)trigger_type) + "'");
            break;
        }

        case TriggerType::WIDGET_ALL: {
            // A interface page may have a global update function in the likes of shouldOpen() or shouldClose().
            auto update_interface_page = functionality["update"];
            if(!update_interface_page.isNil())
                update_interface_page();

            // A interface page may have a global render function in the likes of shouldOpen() or shouldClose().
            auto render_interface_page = functionality["render"];
            if(!render_interface_page.isNil())
                render_interface_page();

            for(int i = 1; i <= length; i++) {
                auto table = components[i];
                auto id = lua::readString(table["id"], true);

                auto component = this->getComponent(id);
                if(component && component.get()->hasEventOverride(event_name)) {
                    auto event = table[event_name];
                    event();
                }
            }

            break;
        }

        case TriggerType::WIDGET_SELECTED: {
            auto current_component = this->getCurrentComponent();
            if(current_component && current_component.get()->hasEventOverride(event_name)) {
                auto gamestate = game_manager.gamestate.getGamestate();

                for(int i = 1; i <= length; i++) {
                    auto table = components[i];
                    auto id = lua::readString(table["id"], true);
                    if(id == current_component.get()->getWidgetID()) {
                        auto event = table[event_name];
                        
                        if(event_name == "onKeyPress")
                            event(gamestate->controls.last_key_name);

                        else
                            event();
                        return;
                    }
                }
            }

            break;
        }
    }
}

void InterfacePage::checkShouldClose() const {
    if(this->isVisible()) {
        lua::runLuaFile(this->functionality_filename);
        auto functionality = luabridge::getGlobal(game_manager.lua(), "Functionality");
        auto should_close = functionality["shouldClose"];

        if(should_close.isNil()) {
            iso::printError("InterfacePage::checkShouldClose()", "Unimplemented mandatory function 'shouldClose' in '" + this->definition_filename + "'");
            return;
        }
        
        if(!should_close.isNil())
            should_close();
    }
}

void InterfacePage::checkShouldOpen() const {
    if(!this->isVisible()) {
        lua::runLuaFile(this->functionality_filename);
        auto functionality = luabridge::getGlobal(game_manager.lua(), "Functionality");
        auto should_open = functionality["shouldOpen"];

        if(should_open.isNil()) {
            iso::printError("InterfacePage::checkShouldOpen()", "Unimplemented mandatory function 'shouldOpen' in '" + this->definition_filename + "'");
            return;
        }
        
        if(!should_open.isNil())
            should_open();
    }
}

AbstractComponent InterfacePage::getCurrentComponent() const {
    auto gamestate = game_manager.gamestate.getGamestate();

    std::vector <AbstractComponent> possible;

    for(const AbstractComponent& component : this->interface) {
        if(component.get()->containsPoint(gamestate->mouse_position_interface)) {
            possible.push_back(component);
            if(component.get()->hasChildren()) {
                for(const auto& pair : component.get()->getWidgetData().components) {
                    const auto& child = pair.second;
                    if(child.get()->containsPoint(gamestate->mouse_position_interface))
                        possible.push_back(child);
                }
            }
        }
    }

    if(!possible.size())
        return nullptr;

    auto compare = [](AbstractComponent component1, AbstractComponent component2) {
        return component1.get()->getDrawPriority() < component2.get()->getDrawPriority();
    };

    std::sort(possible.begin(), possible.end(), compare);
    return possible.back();
}
}