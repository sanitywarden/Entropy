#include "widgetRegion.hpp"
#include "worldmap.hpp"

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
    auto* widget = static_cast<Widget*>(this->interface.at("widget_region"));
    if(widget)
        target.draw(*widget);

    auto* button_travel = static_cast<Button*>(this->interface.at("button_travel"));
    if(button_travel)
        target.draw(*button_travel);

    auto* label = static_cast<Label*>(this->interface.at("text_region_index"));
    if(label)
        target.draw(*label);
}

void WidgetRegion::createUI() {
    sf::Vector2i t_widget_size(3, 2);
    sf::Vector2i t_button_size(8, 4);
    int window_width  = this->manager->window.windowWidth();
    int window_height = this->manager->window.windowHeight();

    static Widget widget_body(this->manager, t_widget_size);
        sf::Vector2f widget_size = widget_body.getWidgetSize();
        widget_body.setWidgetID("widget_region");
        widget_body.setWidgetPosition(0, window_height - widget_size.y);
        sf::Vector2f widget_position = widget_body.getWidgetPosition();

    static Button button_travel(this->manager, t_button_size, "Visit");
        sf::Vector2f button_size = button_travel.getWidgetSize();
        button_travel.setWidgetID("button_travel");
        button_travel.setWidgetPosition(widget_position + widget_size - button_size);
        button_travel.label.setWidgetPosition(button_travel.getWidgetPosition() + sf::Vector2f(button_size.x / 2, button_size.y / 2));

    this->interface.insert({ widget_body.getWidgetID()  , &widget_body   });
    this->interface.insert({ button_travel.getWidgetID(), &button_travel });
}

void WidgetRegion::updateUI() {
    // Update button_travel text.
    
    Worldmap* worldmap = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());
    int index = worldmap->getSelectedIndex();

    if(index < 0 || index > this->manager->world.getWorldSize())
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

    static Label text_region_index(this->manager);
        text_region_index.setWidgetID("text_region_index");
        text_region_index.setString(data);
        text_region_index.setWidgetPosition(widget_position + sf::Vector2f(0.05f * widget_size.x, 0.0375f * widget_size.y));
    
    this->interface.insert({ text_region_index.getWidgetID(), &text_region_index });
}

void WidgetRegion::functionality() {
    Worldmap* worldmap    = static_cast<Worldmap*>(this->manager->gamestate.getGamestate());
    Button* button_travel = static_cast<Button*>(this->getComponent("button_travel"));

    if(worldmap->selected_index != -1 && worldmap->mouse_pressed && !worldmap->mouse_drag && button_travel->containsPoint(worldmap->mouse_position_interface)) {
        Region& region = this->manager->world.world_map[worldmap->selected_index]; 

        // You check for these things here to avoid calling functions responsible for world generation.
        // Biomes: arctic, ocean and sea do not have planned content.
        if(region.biome.biome_name == BIOME_ARCTIC.biome_name || region.biome.biome_name == BIOME_OCEAN.biome_name || region.biome.biome_name == BIOME_SEA.biome_name) {
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
            this->manager->world.generateRegion(worldmap->selected_index, region);

        regionmap->setCurrentRegion(worldmap->selected_index);
        this->manager->gamestate.setGamestate("regionmap");
        return;
    }
}