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
    static Label text(this->manager);
        text.setWidgetID("label_debug_performance");
        text.setWidgetPosition(0, 0);

    this->interface.insert({ text.getWidgetID(), &text });
}

void DebugPerformance::updateUI() {
    const int fps         = this->manager->getFramesPerSecond();
    const int time        = this->manager->getTimePerFrame();

    std::string data;
    data += "Frames per second: " + std::to_string(fps)         + "\n";
    data += "Time per frame: "    + std::to_string(time)        + "ms\n";

    /*  Current gamestate.
     *  Check what gamestate is it and display adequate data. */
    auto* gamestate = this->manager->gamestate.getGamestate();
    std::string gamestate_id = gamestate->state_id;
    
    data += "Mouse position: "  + std::to_string((int)gamestate->mouse_position_window.x)    + " / " + std::to_string((int)gamestate->mouse_position_window.y)    + "\n"; 
    data += "Mouse interface: " + std::to_string((int)gamestate->mouse_position_interface.x) + " / " + std::to_string((int)gamestate->mouse_position_interface.y) + "\n"; 
    data += "Time passed: "     + std::to_string(this->manager->time) + "\n";

    if(gamestate_id == "Worldmap") {
        auto* worldmap = static_cast<Worldmap*>(gamestate);
        
        const int current_index  = worldmap->getCurrentIndex();
        const int selected_index = worldmap->getSelectedIndex();
        const int draw_calls     = worldmap->getDrawCalls();

        data += "Current index:  " + std::to_string(current_index)  + "\n";
        data += "Selected index: " + std::to_string(selected_index) + "\n";
        data += "Selected unit: "  + std::to_string(worldmap->selected_unit_id) + "\n";
        data += "Draw calls: "     + std::to_string(draw_calls) + "\n";
        data += "Terrain: "        + std::to_string(this->manager->world.world_map[current_index].regiontype.is_terrain()) + "\n";
        data += "Coast: "          + std::to_string(this->manager->world.world_map[current_index].regiontype.is_coast())   + "\n";
    }

    if(gamestate_id == "Regionmap") {
        auto* regionmap = static_cast<Regionmap*>(gamestate);

        const int current_index = regionmap->getCurrentIndex();
        const int draw_calls    = regionmap->getDrawCalls();
        const auto* region      = regionmap->getCurrentRegion();

        data += "Current index: "        + std::to_string(current_index)                        + "\n";
        data += "Draw calls: "           + std::to_string(draw_calls)                           + "\n";
        data += "Tree quantity: "        + std::to_string(region->trees.size())                 + "\n";
        data += "Building quantity: "    + std::to_string(region->buildings.size())             + "\n";
        data += "Population quantitiy: " + std::to_string(region->population.size())            + "\n";
        data += "Tile elevation: "       + std::to_string(region->map[current_index].elevation) + "\n";

        data += "Gold: "  + std::to_string(region->resources.gold)  + "\n";
        data += "Wood: "  + std::to_string(region->resources.wood)  + "\n";
        data += "Stone: " + std::to_string(region->resources.stone) + "\n";
        data += "Food: "  + std::to_string(region->resources.food)  + "\n";
    }

    auto* text = static_cast<Label*>(this->interface.at("label_debug_performance"));
    text->setString(data);
}

void DebugPerformance::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->manager == nullptr)
        return;

    for(const auto& pair : this->interface) {
        auto* component = pair.second;

        if(component)
            target.draw(*component);
    }
}