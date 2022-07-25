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

    this->show = false;
}

WidgetUnit::~WidgetUnit() {

}

void WidgetUnit::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;

    auto* worldmap = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());
    if(worldmap->selected_unit_id == -1)
        return;

    auto* widget = static_cast<Widget*>(this->getComponent("widget_unit"));
    if(widget)
        target.draw(*widget);

    auto* unit = this->manager->getHumanPlayer()->getUnit(worldmap->selected_unit_id);
    if(unit->isSameType(UnitType::UNIT_SETTLER)) {
        auto* button_colonise = static_cast<Button*>(this->getComponent("button_colonise"));
        button_colonise->show = this->manager->regionCanBeColonised(unit->current_index);
        if(button_colonise->show) {
            target.draw(*button_colonise);
        }
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

    auto* unit = this->manager->getHumanPlayer()->getUnit(selected_unit_id);
    if(!unit)
        return;

    std::string data = "";
    data += this->manager->getHumanPlayer()->getCountryName() + " | " + unit->getUnitName() + "\n";
    data += "Currently stationed at #" + std::to_string(unit->current_index) + "\n";

    auto* label_component = static_cast<Label*>(this->getComponent("text_widget_unit"));
    label_component->setString(data);
}

void WidgetUnit::functionality() {
    auto* worldmap = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());
    if(worldmap->selected_unit_id == -1)
        return;

    auto* unit = this->manager->getUnit(worldmap->selected_unit_id);
    if(!unit)
        return;
    
    auto region_index = unit->current_index;  
    auto& region      = this->manager->world.world_map[region_index];

    if(unit) {
        auto* button_dismantle = static_cast<Button*>(this->getComponent("button_dismantle"));    
        if(worldmap->controls.mouseLeftPressed() && button_dismantle->containsPoint(worldmap->mouse_position_interface)) {
            this->manager->deleteUnit(unit->getID());
        }
    }

    if(unit) {
        if(unit->isSameType(UnitType::UNIT_SETTLER)) {
            auto* button_colonise = this->getComponent("button_colonise");
            
            if(this->manager->regionCanBeColonised(region_index) && worldmap->controls.mouseLeftPressed() && button_colonise->containsPoint(worldmap->mouse_position_interface)) {
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
                worldmap->block_keybinds = true;

                auto* city_settle_gui = static_cast<WidgetSettleCity*>(worldmap->getInterfaceComponent("component_widget_settle_city"));
                city_settle_gui->region_id = region_index;

                // After colonising, it should be deleted.
                this->manager->deleteUnit(unit->getID());

                worldmap->setVisibilityFalse("component_widget_unit");
            }
        }
    }
}
