#include "gui/performance.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"

using namespace gui;
using namespace iso;

DebugPerformance::DebugPerformance() : InterfacePage(nullptr) {

}

DebugPerformance::DebugPerformance(SimulationManager* manager) : InterfacePage(manager) {
    this->setWidgetID("component_debug_performance");
    this->createUI();
}

DebugPerformance::~DebugPerformance() {

}

void DebugPerformance::createUI() {
    auto text = LabelComponent(new Label(this->manager));
        text.get()->setWidgetID("label_debug_performance");
        text.get()->setWidgetPosition(0, 0);

    this->addComponent(text);
}

void DebugPerformance::updateUI() {
    const int fps  = this->manager->getFramesPerSecond();
    const int time = this->manager->getTimePerFrame();

    std::string data;
    data += "Frames per second: " + std::to_string(fps)         + "\n";
    data += "Time per frame: "    + std::to_string(time)        + "ms\n";

    /*  Current gamestate.
     *  Check what gamestate is it and display adequate data. */
    auto* gamestate = this->manager->gamestate.getGamestate();
    std::string gamestate_id = gamestate->state_id;
    
    auto view_world     = gamestate->view_game;
    auto view_interface = gamestate->view_interface;

    data += "Mouse position: "  + std::to_string((int)gamestate->mouse_position_window.x)    + " / " + std::to_string((int)gamestate->mouse_position_window.y)    + "\n"; 
    data += "Mouse interface: " + std::to_string((int)gamestate->mouse_position_interface.x) + " / " + std::to_string((int)gamestate->mouse_position_interface.y) + "\n"; 
    data += "Time passed: "     + std::to_string(this->manager->time) + "\n";
    data += "Date DMY: "        + this->manager->getDateFormatted() + "\n";
    data += "Draw calls: "      + std::to_string(this->manager->getDrawCalls()) + "\n";
    data += "View interface: "  + std::to_string((int)view_interface.getSize().x) + " " + std::to_string((int)view_interface.getSize().y) + "\n";
    data += "View game: "       + std::to_string((int)view_world.getSize().x)     + " " + std::to_string((int)view_world.getSize().y) + "\n";

    if(gamestate_id == "Worldmap") {
        auto* worldmap = static_cast<Worldmap*>(gamestate);
        
        const int current_index  = worldmap->getCurrentIndex();
        const int selected_index = worldmap->getSelectedIndex();

        data += "Current index:  " + std::to_string(current_index)  + "\n";
        data += "Selected index: " + std::to_string(selected_index) + "\n";
        data += "Selected unit: "  + std::to_string(worldmap->selected_unit_id) + "\n";
        data += "Terrain: "        + std::to_string(this->manager->world.world_map[current_index].regiontype.is_terrain()) + "\n";
        data += "Coast: "          + std::to_string(this->manager->world.world_map[current_index].regiontype.is_coast())   + "\n";
        data += "Forest: "         + std::to_string(this->manager->world.forests.count(current_index)) + "\n";
        data += "Lake: "           + std::to_string(this->manager->world.is_lake(current_index)) + "\n";
    }

    if(gamestate_id == "Regionmap") {
        auto* regionmap = static_cast<Regionmap*>(gamestate);

        const int current_index  = regionmap->getCurrentIndex();
        const auto* region       = regionmap->getCurrentRegion();
        const auto& current_tile = region->map[current_index];

        auto grid_position = this->manager->world.tileGridPosition(gamestate->mouse_position_window);
        
        auto* widget_menu = static_cast<WidgetMenuBuilding*>(regionmap->getInterfaceComponent("component_widget_menu_building"));
        auto selected_building = widget_menu->getBuilding();

        data += "Current index: "        + std::to_string(current_index)                        + "\n";
        data += "Selected: "             + std::to_string(grid_position.x) + " " + std::to_string(grid_position.y) + "\n";
        data += "Selected building: "    + selected_building.getName() + "\n";

        data += "Tree quantity: "        + std::to_string(region->trees.size())                 + "\n";
        data += "Building quantity: "    + std::to_string(region->buildings.size())             + "\n";
        data += "Population quantitiy: " + std::to_string(region->population.size())            + "\n";
        
        data += "Tile elevation: "       + std::to_string(region->map[current_index].getElevation()) + "\n";
        data += "Tile position: "        + std::to_string((int)current_tile.getPosition().x) + " " + std::to_string((int)current_tile.getPosition().y) + " " + std::to_string((int)current_tile.getPosition().z) + "\n";
        data += "River: "                + std::to_string(current_tile.tiletype.is_river())   + "\n";
        data += "Ocean: "                + std::to_string(current_tile.tiletype.is_ocean())   + "\n";
        data += "Water: "                + std::to_string(current_tile.tiletype.is_water())   + "\n";
        data += "Terrain: "              + std::to_string(current_tile.tiletype.is_terrain()) + "\n";
    }

    auto* text = static_cast<Label*>(this->getComponent("label_debug_performance"));
    text->setString(data);
}

void DebugPerformance::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->manager == nullptr)
        return;

    for(const auto& pair : this->interface) {
        auto* component = pair.second.get();

        if(component)
            target.draw(*component);
    }
}