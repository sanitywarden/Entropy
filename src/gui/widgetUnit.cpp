#include "widgetUnit.hpp"
#include "worldmap.hpp"
#include "generationSettings.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

WidgetUnit::WidgetUnit() {
    this->manager = nullptr;
    std::cout << "[GUI][WidgetUnit]: Simulation manager is a nullptr.\n";
}

WidgetUnit::WidgetUnit(SimulationManager* manager) : InterfacePage(manager) {
    this->setWidgetID("component_widget_unit");
    this->createUI();

    this->unit = nullptr;
    this->show = false;
}

WidgetUnit::~WidgetUnit() {

}

void WidgetUnit::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;

    if(!this->unit)
        return;

    auto* widget = static_cast<Widget*>(this->getComponent("widget_unit"));
    if(widget)
        target.draw(*widget);

    if(unit->isSameType(UNIT_SETTLER)) {
        auto* button_colonise = static_cast<Button*>(this->getComponent("button_colonise"));
        if(button_colonise->show)
            target.draw(*button_colonise);
    }

    auto* button_dismantle = static_cast<Button*>(this->getComponent("button_dismantle"));
    if(button_dismantle->show)
        target.draw(*button_dismantle);

    auto* label_component = static_cast<Label*>(this->getComponent("text_widget_unit"));
    if(label_component)
        target.draw(*label_component);
} 

void WidgetUnit::createUI() {
    auto window_size = this->manager->window.windowSize();
    
    sf::Vector2i t_widget_size(
        window_size.x / 400,
        window_size.y / 500
    );

    sf::Vector2i t_button_size(
        window_size.x / 200,
        window_size.y / 250
    );

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        sf::Vector2f widget_size = widget_body.get()->getWidgetSize();
        widget_body.get()->setWidgetID("widget_unit"); 
        widget_body.get()->setWidgetPosition(window_size - widget_size);
        sf::Vector2f widget_position = widget_body.get()->getWidgetPosition();

    auto button_dismantle = ButtonComponent(new Button(this->manager, t_button_size, "Dismantle"));
        sf::Vector2f button_size = button_dismantle.get()->getWidgetSize();
        sf::Vector2f half_button_size = sf::Vector2f(button_size.x / 2, button_size.y / 2);
        button_dismantle.get()->setWidgetID("button_dismantle");
        button_dismantle.get()->setWidgetPosition(widget_position + widget_size - button_size);
        button_dismantle.get()->label.setWidgetPosition(button_dismantle.get()->getWidgetPosition() + half_button_size);
        button_dismantle.get()->show = true;

    auto button_colonise = ButtonComponent(new Button(this->manager, t_button_size, "Colonise"));
        button_colonise.get()->setWidgetID("button_colonise");
        button_colonise.get()->setWidgetPosition(widget_position + widget_size - sf::Vector2f(button_size.x, button_size.y) - sf::Vector2f(0, button_size.y));
        button_colonise.get()->label.setWidgetPosition(button_colonise.get()->getWidgetPosition() + half_button_size);
    
    auto widget_text = LabelComponent(new Label(this->manager, ""));
        widget_text.get()->setWidgetID("text_widget_unit");
        widget_text.get()->setWidgetPosition(widget_position + sf::Vector2f(8, 8));

    this->addComponent(widget_body);
    this->addComponent(button_dismantle);
    this->addComponent(button_colonise);
    this->addComponent(widget_text);
}

void WidgetUnit::updateUI() {
    auto* worldmap = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());
    int selected_unit_id = worldmap->getSelectedUnitID();

    this->unit = this->manager->getHumanPlayer()->getUnit(selected_unit_id);
    if(!this->unit)
        return;

    std::string data = "";
    data += this->manager->getHumanPlayer()->getCountryName() + " | " + this->unit->getUnitName() + "\n";
    data += "Currently stationed at #" + std::to_string(this->unit->current_index) + "\n";

    auto* label_component = static_cast<Label*>(this->getComponent("text_widget_unit"));
    label_component->setString(data);

    // If the terrain is not colonised, display the "colonise" button.

    auto* button_colonise = this->getComponent("button_colonise");
    button_colonise->show = this->canColonise(this->unit->current_index);
}

void WidgetUnit::functionality() {
    if(!this->unit)
        return;

    auto* worldmap    = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());
    auto region_index = this->unit->current_index;  
    auto& region      = this->manager->world.world_map[region_index];

    if(this->unit) {
        auto* button_dismantle = static_cast<Button*>(this->getComponent("button_dismantle"));
        
        if(worldmap->controls.mouseLeftPressed() && button_dismantle->containsPoint(worldmap->mouse_position_interface)) {
            this->deleteCurrentUnit();
        }
    }

    if(this->unit) {
        if(unit->isSameType(UNIT_SETTLER)) {
            auto* button_colonise = this->getComponent("button_colonise");
            
            if(this->canColonise(region_index) && worldmap->controls.mouseLeftPressed() && button_colonise->containsPoint(worldmap->mouse_position_interface)) {
                // Region to be added to the player's territory.
                auto* human_player = this->manager->getHumanPlayer();

                // Does not yet have a settlement.
                if(!human_player->empireSize()) {
                    human_player->setCapital(region_index);
                }
                
                region.owner = human_player;
                human_player->addOwnedRegion(region_index);
                auto colour_transparent = human_player->getTeamColourTransparent();
                region.object_colour = colour_transparent;

                worldmap->setVisibilityTrue("component_widget_settle_city");

                auto* city_settle_gui = static_cast<WidgetSettleCity*>(worldmap->getInterfaceComponent("component_widget_settle_city"));
                city_settle_gui->region_id = region_index;

                // After colonising, it should be deleted.
                this->deleteCurrentUnit();

                worldmap->setVisibilityFalse("component_widget_unit");
            }
        }
    }
}

bool WidgetUnit::canColonise(int index) const {
    if(index == -1)
        return false;

    const auto& region = this->manager->world.world_map[index];
    if(region.isOwned()) 
        return false;
    
    if(region.regiontype.is_ocean())
        return false;

    return true;
}

void WidgetUnit::deleteCurrentUnit() {
    auto* human_player = this->manager->getHumanPlayer();
    auto& units        = human_player->units;
    auto region_index  = this->unit->current_index;  
    auto& region       = this->manager->world.world_map[region_index];
    auto* worldmap     = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());

    for(auto it = units.begin(); it != units.end(); ++it) {
        if((*it).get()->getID() == this->unit->getID()) {
            units.erase(it);
            this->unit  = nullptr;
            region.unit = nullptr;
            worldmap->selected_unit_id = -1;
            return;
        }
    }
}