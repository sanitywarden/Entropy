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
    auto window_size = this->manager->window.windowSize();
    auto ratio = sf::Vector2i(
        window_size.x / 300,
        window_size.y / 100
    );

    sf::Vector2i t_widget_size(ratio);

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        sf::Vector2f widget_size = widget_body.get()->getWidgetSize();
        widget_body.get()->setWidgetID("widget_menu_building");
        widget_body.get()->setWidgetPosition(window_size - widget_size);
        sf::Vector2f widget_position = widget_body.get()->getWidgetPosition();

    this->addComponent(widget_body);

    auto image_size = sf::Vector2f(48, 48);
    int building_no = 0;
    
    for(const auto& building : BUILDING_LOOKUP_TABLE) {
        if(*building != BUILDING_EMPTY) {
            auto image = ImageComponent(new ImageHolder(this->manager, building.get()->getBuildingMenuIconName()));
            image.get()->setWidgetID("imageholder_" + building.get()->getTextureName());

            auto final_position = calculateItemPosition(building_no, BUILDING_LOOKUP_TABLE.size());       

            image.get()->setWidgetPosition(final_position);
            image.get()->setWidgetSize(image_size);

            this->addComponent(image);
            
            building_no++;
        }
    }
}

sf::Vector2f WidgetMenuBuilding::calculateItemPosition(int building_no, int building_total) {
        sf::Vector2f widget_position = this->getComponent("widget_menu_building")->getWidgetPosition();
        sf::Vector2f widget_size     = this->getComponent("widget_menu_building")->getWidgetSize();
        sf::Vector2f image_size      = this->manager->resource.getTextureSize("icon_default");
        sf::Vector2f offset     (widget_size.x / 20, widget_size.x / 20);
        sf::Vector2f space_const(widget_size.x / 40, widget_size.y / 40);

        int row_size = 0;
        for(const auto& building : BUILDING_LOOKUP_TABLE) {
            int a1 = widget_position.x + offset.x;
            int n  = row_size;
            int r  = image_size.x + space_const.x;

            int lim = widget_position.x + widget_size.x - offset.x;
            int term = (n == 0)
                ? a1
                : a1 + (n - 1) * r;

            if(term + image_size.x < lim)
                row_size++;
            
            else
                break;
        }

        int space = (widget_size.x + -2 * offset.x + -row_size * image_size.x) / (row_size - 1);
        int x     = building_no % row_size;
        int y     = building_no / row_size;
        int ri    = building_no - (y * row_size); // Item index in the row.
        int hi    = row_size / 2;                 // Half items.

        sf::Vector2f position;
        if(ri < hi || building_total < row_size || ri < row_size) {
            position =
            widget_position
            + offset
            + sf::Vector2f(x * image_size.x, y * image_size.y)
            + sf::Vector2f(x * space       , y * space);
        }

        else if(ri >= hi) {
            position = 
            widget_position
            + sf::Vector2f(widget_size.x, 0)
            + sf::Vector2f(-offset.x, offset.y)
            + sf::Vector2f(-image_size.x, 0)
            + sf::Vector2f(-(x - hi) * image_size.x, y * image_size.y)
            + sf::Vector2f(-(x - hi) * space       , y * space);
        }

        // Row size is 2n + 1.
        // Middle item.
        else if(ri == hi + 1 && row_size % 2 == 1) {
            auto n_left  = this->calculateItemPosition(building_no - 1, building_total);
            auto n_right = this->calculateItemPosition(building_no + 1, building_total);

            auto middle_item_offset = (n_right.x - (n_left.x + image_size.x) - image_size.x) / 2;
            position = n_left + sf::Vector2f(middle_item_offset, 0);
        }

        return position;
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

std::shared_ptr <iso::Building> WidgetMenuBuilding::getBuilding() {
    const std::string& building_name = this->last_selected_building;
    for(const auto& building : BUILDING_LOOKUP_TABLE) {
        if(*building.get() != BUILDING_EMPTY && building_name == building.get()->getTextureName())
            return building;
    }

    // Empty building.
    return BUILDING_LOOKUP_TABLE[0];
}

void WidgetMenuBuilding::resetBuilding() {
    this->last_selected_building = "Empty";
} 