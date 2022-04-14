#include "widgetRegion.hpp"
#include "worldmap.hpp"
#include "generationSettings.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

WidgetRegion::WidgetRegion() {
    this->manager = nullptr;
    std::cout << "[GUI][WidgetRegion]: Simulation manager is a nullptr.\n";
}

WidgetRegion::WidgetRegion(SimulationManager* manager) : InterfacePage(manager) {
    this->setWidgetID("component_widget_region");
    this->createUI();
}

WidgetRegion::~WidgetRegion() {

}

void WidgetRegion::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->manager == nullptr)
        return;

    // You always draw the main widget first,
    // so that there is no issue with components overlapping.
    auto* widget = static_cast<Widget*>(this->getComponent("widget_region"));
    if(widget)
        target.draw(*widget);

    auto* button_travel = static_cast<Button*>(this->getComponent("button_travel"));
    if(button_travel)
        target.draw(*button_travel);

    auto* label = static_cast<Label*>(this->getComponent("text_region_index"));
    if(label)
        target.draw(*label);
}

void WidgetRegion::createUI() {
    sf::Vector2i t_widget_size(3, 2);
    sf::Vector2i t_button_size(8, 4);
    int window_width  = this->manager->window.windowWidth();
    int window_height = this->manager->window.windowHeight();

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        sf::Vector2f widget_size = widget_body.get()->getWidgetSize();
        widget_body.get()->setWidgetID("widget_region");
        widget_body.get()->setWidgetPosition(0, window_height - widget_size.y);
        sf::Vector2f widget_position = widget_body.get()->getWidgetPosition();

    auto button_travel = ButtonComponent(new Button(this->manager, t_button_size, "Visit"));
        sf::Vector2f button_size = button_travel.get()->getWidgetSize();
        button_travel.get()->setWidgetID("button_travel");
        button_travel.get()->setWidgetPosition(widget_position + widget_size - button_size);
        button_travel.get()->label.setWidgetPosition(button_travel.get()->getWidgetPosition() + sf::Vector2f(button_size.x / 2, button_size.y / 2));

    this->addComponent(widget_body);
    this->addComponent(button_travel);
}

void WidgetRegion::updateUI() {
    // Update button_travel text.
    
    Worldmap* worldmap = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());
    int index = worldmap->getSelectedIndex();

    if(!world_settings.inWorldBounds(index))
        return;

    Region& region = this->manager->world.world_map[index];
    Widget& widget = *(static_cast<Widget*>(this->getComponent("widget_region")));
    sf::Vector2f widget_position = widget.getWidgetPosition();
    sf::Vector2f widget_size     = widget.getWidgetSize();

    std::string data;
    std::string forest = this->manager->world.forests.count(index) ? "True" : "False";
    std::string river  = this->manager->world.rivers.count(index)  ? "True" : "False";
    std::string owned  = region.isOwned() ? "True" : "False";
    data += "Index: "  + std::to_string(index)   + "\n";
    data += "Biome: "  + region.biome.biome_name + "\n";
    data += "Forest: " + forest + "\n";
    data += "River: "  + river  + "\n";
    data += "Owned: "  + owned  + "\n";
    
    if(region.isOwned())
        data += "Owner: " + region.owner->getCountryName() + "\n";

    auto text_region_index = LabelComponent(new Label(this->manager));
        text_region_index.get()->setWidgetID("text_region_index");
        text_region_index.get()->setString(data);
        text_region_index.get()->setWidgetPosition(widget_position + sf::Vector2f(0.05f * widget_size.x, 0.0375f * widget_size.y));
    
    this->addComponent(text_region_index);
}

void WidgetRegion::functionality() {
    Worldmap* worldmap    = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());
    Button* button_travel = static_cast<Button*>(this->getComponent("button_travel"));

    if(worldmap->selected_index != -1 && worldmap->controls.mouseLeftPressed() && !worldmap->mouse_drag && button_travel->containsPoint(worldmap->mouse_position_interface)) {
        Region& region = this->manager->world.world_map[worldmap->selected_index]; 

        // You check for these things here to avoid calling functions responsible for world generation.
        // Biomes: arctic, ocean and sea do not have planned content.
        if(region.biome == BIOME_OCEAN) {
            std::cout << "[Button Visit Region]: Requested to generate a region not meant for visiting.\n";
            worldmap->selected_index = -1;
            return;
        }

        if(!this->manager->gamestate.checkGamestateExists("regionmap")) {
            static Regionmap regionmap_gamestate(this->manager);
            this->manager->gamestate.addGamestate("regionmap", regionmap_gamestate);
        }

        // Get a pointer to the gamestate.
        // You do not need to check if it's a nullptr, because it can not be, but do it just to be sure. 
        // It will make debugging easier in case of a unexpected behaviour (function returns nullptr).
        Regionmap* regionmap = static_cast<Regionmap*>(this->manager->gamestate.getGamestateByName("regionmap"));
        if(!regionmap) {
            std::cout << "[Button Visit Region]: Gamestate regionmap is a nullptr.\n";
            this->manager->exitApplication(1);
        }
    
        // You have to generate the region first, because setCurrentRegion() depends on it being generated.
        if(!region.visited)
            this->manager->world.generateRegion(worldmap->selected_index);

        regionmap->setCurrentRegion(worldmap->selected_index);
        this->manager->gamestate.setGamestate("regionmap");
        return;
    }
}