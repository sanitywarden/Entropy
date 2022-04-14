#include "widgetUnit.hpp"
#include "worldmap.hpp"
#include "generationSettings.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

WidgetUnit::WidgetUnit() {
    this->manager = nullptr;
    std::cout << "[GUI][WidgetUnit]: Simulation manager is a nullptr.\n";

    this->unit = nullptr;
}

WidgetUnit::WidgetUnit(SimulationManager* manager) : InterfacePage(manager) {
    this->setWidgetID("component_widget_unit");
    this->createUI();

    this->unit = nullptr;
}

WidgetUnit::~WidgetUnit() {

}

void WidgetUnit::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->manager == nullptr)
        return;

    if(this->unit == nullptr)
        return;

    auto* widget = static_cast<Widget*>(this->getComponent("widget_unit"));
    if(widget)
        target.draw(*widget);

    if(this->unit->getName() == "settler") {
        auto* button_colonise = static_cast<Button*>(this->getComponent("button_colonise"));
        if(button_colonise)
            target.draw(*button_colonise);
    }
} 

void WidgetUnit::createUI() {
    sf::Vector2i t_widget_size(3, 2);
    sf::Vector2i t_button_size(8, 4);
    int window_width  = this->manager->window.windowWidth();
    int window_height = this->manager->window.windowHeight();

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        sf::Vector2f widget_size = widget_body.get()->getWidgetSize();
        sf::Vector2f window_size = sf::Vector2f(window_width, window_height);
        widget_body.get()->setWidgetID("widget_unit"); 
        widget_body.get()->setWidgetPosition(window_size - widget_size);
        sf::Vector2f widget_position = widget_body.get()->getWidgetPosition();

    auto button_colonise = ButtonComponent(new Button(this->manager, t_button_size, "Colonise"));
        sf::Vector2f button_size = button_colonise.get()->getWidgetSize();
        button_colonise.get()->setWidgetID("button_colonise");
        button_colonise.get()->setWidgetPosition(widget_position + widget_size - sf::Vector2f(button_size.x, button_size.y));
        button_colonise.get()->label.setWidgetPosition(widget_position + widget_size - sf::Vector2f(button_size.x / 2, button_size.y / 2));

    this->addComponent(widget_body);
    this->addComponent(button_colonise);
}

void WidgetUnit::updateUI() {
    Worldmap* worldmap = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());
    int selected_unit_id = worldmap->getSelectedUnitID();

    if(selected_unit_id != -1) {
        auto& human_player = this->manager->getHumanPlayer();
        auto* selected_unit = human_player.getUnit(selected_unit_id);
        this->unit = selected_unit;
    }
    
    else this->unit = nullptr;
}

void WidgetUnit::functionality() {
    if(!this->unit)
        return;

    auto* worldmap = this->manager->gamestate.getGamestate();
    auto* button_colonise = this->getComponent("button_colonise");

    auto region_index = this->unit->current_index;  
    auto& region      = this->manager->world.world_map[region_index];

    if(this->canColonise(region_index) && this->unit && this->unit->getName() == "settler" && worldmap->controls.mouseLeftPressed() && button_colonise->containsPoint(worldmap->mouse_position_interface)) {
        // Region to be added to the player's territory.
        auto& human_player = this->manager->getHumanPlayer();
        human_player.addOwnedRegion(region_index);

        region.owner = &human_player;
        region.object_colour = human_player.getTeamColour();
    }
}

bool WidgetUnit::canColonise(int index) {
    if(index == -1)
        return false;

    const std::vector <int>& owned_regions = this->manager->getHumanPlayer().readOwnedRegions();
    const Region& region = this->manager->world.world_map[index];

    if(region.owner)
        return false;

    for(auto i : owned_regions) {
        if(region.biome == BIOME_OCEAN) {
            return false;
        }

        if(index == i - 1 ||
           index == i + 1 ||
           index == i - world_settings.getWorldWidth() ||
           index == i + world_settings.getWorldWidth()
        ) {
            return true;
        }
    }

    return false;
}