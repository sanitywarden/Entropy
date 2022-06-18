#include "tooltip.hpp"
#include "gamestate.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"
#include "gui/widgetMenuBuilding.hpp"

using namespace gui;
using namespace iso;

Tooltip::Tooltip() 
    : InterfacePage(nullptr)
{
    std::cout << "[GUI][Tooltip]: Simulation manager is a nullptr.\n";
}

Tooltip::Tooltip(SimulationManager* manager)
    : InterfacePage(manager)
{
    this->setWidgetID("component_tooltip");
    this->createUI();

    this->show = false;
    this->draw_priority = 2;
}

Tooltip::~Tooltip() {

}

void Tooltip::createUI() {
    auto t_widget_size = sf::Vector2i(
        4,
        2
    );

    // Do not set the position.
    // Position will be set based on the hovered GUI item. 

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        widget_body.get()->setBlockTextureBase("widget_black_base");
        widget_body.get()->setWidgetID("widget_body");
        auto widget_size = widget_body.get()->getWidgetSize();
        
    auto widget_text = LabelComponent(new Label(this->manager, ""));
        widget_text.get()->setWidgetID("tooltip_text");

    this->addComponent(widget_body);
    this->addComponent(widget_text);
}

void Tooltip::updateUI() {
    auto* gamestate = this->manager->gamestate.getGamestate();
    auto* text = static_cast<Label*>(this->getComponent("tooltip_text"));

    for(auto& pair : this->interface) {
        auto* component = pair.second.get();
        component->setWidgetPosition(gamestate->mouse_position_interface);
    }

    text->setWidgetPosition(text->getWidgetPosition() + sf::Vector2f(8, 8));
}

void Tooltip::functionality() {
    
}

void Tooltip::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;
    
    if(!this->show)
        return;

    auto* widget_body = static_cast<Widget*>(this->getComponent("widget_body"));
    if(widget_body)
        target.draw(*widget_body);

    auto* widget_text = static_cast<Label*>(this->getComponent("tooltip_text"));
    if(widget_text)
        target.draw(*widget_text);
}

bool Tooltip::intersectsSupportedUI() const {
    auto* gamestate = this->manager->gamestate.getGamestate();
    auto* widget_text = static_cast<Label*>(this->getComponent("tooltip_text"));

    // This is a dirty solution to the problem.

    if(gamestate->state_id == "Worldmap") {
        auto* worldmap = static_cast<Worldmap*>(gamestate);    
    }

    if(gamestate->state_id == "Regionmap") {
        auto* regionmap = static_cast<Regionmap*>(gamestate);

        // Show the tooltip when hovering over images in the regionmap building menu interface.
        if(regionmap->mouseIntersectsUI("component_widget_menu_building")) {
            auto* building_menu = static_cast<WidgetMenuBuilding*>(regionmap->getInterfaceComponent("component_widget_menu_building"));

            for(const auto& pair : building_menu->interface) {
                auto id        = pair.first;
                auto component = pair.second.get();

                if(startsWith(id, "imageholder")) {
                    if(component->containsPoint(regionmap->mouse_position_interface)) {
                        auto building_texture = readAfter(id, "imageholder_");

                        // Empty building.

                        auto building = BUILDING_LOOKUP_TABLE[0];
                        for(auto building_sp : BUILDING_LOOKUP_TABLE) {
                            if(building_sp.get()->getTextureName() == building_texture) {
                                building = building_sp;
                                break;
                            }
                        }

                        // Building has not been found.
                        if(*building == BUILDING_EMPTY)
                            return false;

                        std::string data;
                        data += building.get()->getBuildingName() + "\n";

                        for(const auto& resource : building.get()->getBuildingCost()) {
                            data += resource.getName() + " - " + std::to_string(resource.getQuantity()) + "\n";
                        }

                        widget_text->setString(data);
                        return true;
                    }
                }
            }
        }


    }

    return false;
}