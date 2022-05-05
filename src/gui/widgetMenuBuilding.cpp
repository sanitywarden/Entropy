#include "widgetMenuBuilding.hpp"
#include "regionmap.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

WidgetMenuBuilding::WidgetMenuBuilding() 
    : InterfacePage(nullptr)
{
    std::cout << "[GUI][WidgetMenuBuilding]: Simulation manager is a nullptr.\n";
}

WidgetMenuBuilding::WidgetMenuBuilding(SimulationManager* manager) 
    : InterfacePage(manager) 
{
    this->setWidgetID("component_widget_menu_building");
    this->createUI();

    this->last_selected_building = "Empty";
}

WidgetMenuBuilding::~WidgetMenuBuilding() {

}

void WidgetMenuBuilding::createUI() {
    sf::Vector2i t_widget_size(6, 10);
    int window_width  = this->manager->window.windowWidth();
    int window_height = this->manager->window.windowHeight();

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        sf::Vector2f widget_size = widget_body.get()->getWidgetSize();
        widget_body.get()->setWidgetID("widget_menu_building");
        widget_body.get()->setWidgetPosition(window_width - widget_size.x, window_height - widget_size.y);
        sf::Vector2f widget_position = widget_body.get()->getWidgetPosition();

    this->addComponent(widget_body);

    auto image_size = sf::Vector2f(48, 48);
    auto offset     = sf::Vector2f(image_size.x / 2, image_size.y / 2);
    int building_no = 0;
    
    // Calculate the number of icons possible to fit in this widget.

    int row_size = 0;
    for(const auto& building : BUILDING_LOOKUP_TABLE) {
        int a1 = widget_position.x + offset.x;
        int n  = row_size;
        int r  = row_size * offset.x;

        int lim = widget_position.x + widget_size.x - offset.x;
        int term = (n == 0)
            ? a1
            : a1 + (n - 1) * r;

        if(term < lim)
            row_size++;

        else
            break;  
    }

    for(const auto& building : BUILDING_LOOKUP_TABLE) {
        if(building != BUILDING_EMPTY) {
            auto image = ImageComponent(new ImageHolder(this->manager, building.getBuildingMenuIconName()));
            image.get()->setWidgetID("imageholder_" + building.getTextureName());
            
            const int x = building_no % row_size;
            const int y = building_no / row_size;

            auto final_position = widget_position + offset + sf::Vector2f(x * image_size.x, y * image_size.y) + sf::Vector2f(x * offset.x, 0);

            image.get()->setWidgetPosition(final_position);
            image.get()->setWidgetSize(image_size);

            this->addComponent(image);
            
            building_no++;
        }
    }
}

void WidgetMenuBuilding::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;

    if(!this->show)
        return;

    // Draw the main widget first.
    auto* main_widget = static_cast<Widget*>(this->getComponent("widget_menu_building"));
    if(main_widget)
        target.draw(*main_widget);

    // Draw all the images on top of the main widget.
    for(const auto& pair : this->interface) {
        auto* component = pair.second.get();
        if(component) {
            if(startsWith(component->getWidgetID(), "imageholder")) {
                auto* image_holder = static_cast<ImageHolder*>(component);
                target.draw(*image_holder);
            }
        }
    }
}

/* Returns the texture name of the image under mouse position. 
 * Returns "building_empty" if no image is there. */
void WidgetMenuBuilding::findBuilding() {
    auto* gamestate = this->manager->gamestate.getGamestate(); 
    auto* regionmap = static_cast<Regionmap*>(gamestate);
    auto mouse_position = regionmap->mouse_position_interface;

    for(const auto& pair : this->interface) {
        auto* component = pair.second.get();
        if(component) {
            if(startsWith(component->getWidgetID(), "imageholder")) {
                if(component->containsPoint(mouse_position) && gamestate->controls.mouseLeftPressed()) {
                    auto* image_holder = static_cast<ImageHolder*>(component);
                    auto  widget_id    = image_holder->getWidgetID();
                    auto  texture_name = readAfter(widget_id, "imageholder_");     
                    this->last_selected_building = texture_name;
                }
            }
        }
    }
}

void WidgetMenuBuilding::updateUI() {
    this->findBuilding();
}

Building WidgetMenuBuilding::getBuilding() {
    const std::string& building_name = this->last_selected_building;
    for(const auto& building : BUILDING_LOOKUP_TABLE) {
        if(building != BUILDING_EMPTY && building_name == building.getTextureName())
            return building;
    }

    return BUILDING_EMPTY;
}

void WidgetMenuBuilding::resetBuilding() {
    this->last_selected_building = "Empty";
} 