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
    const int fps   = this->manager->getFramesPerSecond();
    const int time  = this->manager->getTimePerFrame();

    std::string data;
    data += "Frames per second: " + std::to_string(fps)  + "\n";
    data += "Time per frame: "    + std::to_string(time) + "ms\n";

    /*  Current gamestate.
     *  Check what gamestate is it and display adequate data. */
    auto* gamestate = this->manager->gamestate.getGamestate();
    std::string gamestate_id = gamestate->state_id;
    
    data += "Mouse position:  " + std::to_string((int)gamestate->mouse_position_window.x)    + " / " + std::to_string((int)gamestate->mouse_position_window.y)    + "\n"; 
    data += "Mouse interface: " + std::to_string((int)gamestate->mouse_position_interface.x) + " / " + std::to_string((int)gamestate->mouse_position_interface.y) + "\n"; 

    if(gamestate_id == "Worldmap") {
        auto* worldmap = static_cast<Worldmap*>(gamestate);
        
        const int current_index  = worldmap->getCurrentIndex();
        const int selected_index = worldmap->getSelectedIndex();

        data += "Current index:  " + std::to_string(current_index)  + "\n";
        data += "Selected index: " + std::to_string(selected_index) + "\n";
    }

    if(gamestate_id == "Regionmap") {
        auto* regionmap = static_cast<Regionmap*>(gamestate);

        const int current_index = regionmap->getCurrentIndex();

        data += "Current index: " + std::to_string(current_index) + "\n";
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