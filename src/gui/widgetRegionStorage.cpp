#include "widgetRegionStorage.hpp"
#include "regionmap.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

WidgetRegionStorage::WidgetRegionStorage() 
    : InterfacePage(nullptr) 
{
    std::cout << "[GUI][WidgetRegionStorage]: Simulation manager is a nullptr.\n";
}

WidgetRegionStorage::WidgetRegionStorage(SimulationManager* manager)
    : InterfacePage(manager)
{
    this->setWidgetID("component_widget_region_storage");
    this->createUI();

    this->should_refresh = true;
}

WidgetRegionStorage::~WidgetRegionStorage() {

}

void WidgetRegionStorage::createUI() {
    auto window_size = this->manager->window.windowSize();
    auto ratio = sf::Vector2i(
        window_size.x / 300,
        window_size.y / 100
    );

    sf::Vector2i t_widget_size(ratio);
    int window_width  = this->manager->window.windowWidth();
    int window_height = this->manager->window.windowHeight();

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        sf::Vector2f widget_size = widget_body.get()->getWidgetSize();
        widget_body.get()->setWidgetID("widget_menu_items");
        widget_body.get()->setWidgetPosition(window_width - widget_size.x, window_height - widget_size.y);
        sf::Vector2f widget_position = widget_body.get()->getWidgetPosition();

    this->addComponent(widget_body);
}

void WidgetRegionStorage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;

    if(!this->isVisible())
        return;

    // Draw the main widget first.
    auto* main_widget = static_cast<Widget*>(this->getComponent("widget_menu_items"));
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
        
            if(startsWith(component->getWidgetID(), "text_quantity")) {
                auto* label = static_cast<Label*>(component);
                target.draw(*label);
            }
        }
    }
}

void WidgetRegionStorage::updateUI() {
    for(auto pair : this->interface) {
        auto component = pair.second;
        auto component_name = component.get()->getWidgetID();

        if(startsWith(component_name, "imageholder_"))
            this->deleteComponent(component_name);

        if(startsWith(component_name, "text_quantity_"))
            this->deleteComponent(component_name);
    }

    auto image_size = sf::Vector2f(48, 48);
    int resource_no = 0;
    
    auto widget_position = this->getComponent("widget_menu_items")->getWidgetPosition();
    auto widget_size     = this->getComponent("widget_menu_items")->getWidgetSize();

    auto* gamestate = this->manager->gamestate.getGamestate();
    auto* regionmap = static_cast<Regionmap*>(gamestate);
    auto* current_region = regionmap->getCurrentRegion();
    
    int resource_total = 0;
    for(const auto& resource : RESOURCE_LOOKUP_TABLE)
        if(current_region->resources.count(resource.getName()))
            resource_total++;

    for(const auto& resource : RESOURCE_LOOKUP_TABLE) {
        if(current_region->resources.count(resource.getName())) {
            if(!current_region->resources[resource.getName()])
                continue;

            auto image = ImageComponent(new ImageHolder(this->manager, resource.getIcon()));
            auto name_image = "imageholder_" + toLower(resource.getName());
            image.get()->setWidgetID(name_image);
            
            auto text = LabelComponent(new Label(this->manager, std::to_string(current_region->getResourceQuantity(resource))));
            auto name_text = "text_quantity_" + toLower(resource.getName());
            text.get()->setWidgetID(name_text);

            // Image's final position.
            auto position = this->calculateItemPosition(resource_no, resource_total);

            image.get()->setWidgetPosition(position);
            image.get()->setWidgetSize(image_size);

            // Quantity text's final position
            auto text_final_position = position + sf::Vector2f(0, 32) + sf::Vector2f(4, -4); 

            text.get()->setWidgetPosition(text_final_position);

            this->addComponent(image);
            this->addComponent(text);

            resource_no++;
        }

        // If the resource does not exist in the region storage, delete the image.
        else {
            auto name_image = "imageholder_" + toLower(resource.getName());
            this->deleteComponent(name_image);

            auto name_text = "text_quantity_" + toLower(resource.getName());
            this->deleteComponent(name_text);
        }
    }
}

sf::Vector2f WidgetRegionStorage::calculateItemPosition(int resource_no, int resource_total) {
    auto* gamestate = this->manager->gamestate.getGamestate();
    auto* regionmap = static_cast<Regionmap*>(gamestate);
    auto* current_region = regionmap->getCurrentRegion();

    sf::Vector2f widget_position = this->getComponent("widget_menu_items")->getWidgetPosition();
    sf::Vector2f widget_size     = this->getComponent("widget_menu_items")->getWidgetSize();
    sf::Vector2f image_size      = this->manager->resource.getTextureSize("icon_default");
    sf::Vector2f offset     (widget_size.x / 20, widget_size.x / 20);
    sf::Vector2f space_const(widget_size.x / 40, widget_size.y / 40);

    int row_size = 0;
    for(const auto& resource : RESOURCE_LOOKUP_TABLE) {
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
    int x     = resource_no % row_size;
    int y     = resource_no / row_size;
    int ri    = resource_no - (y * row_size); // Item index in the row.
    int hi    = row_size / 2;                 // Half items.

    sf::Vector2f position;
    if(ri < hi || resource_total < row_size || ri < row_size) {
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
        auto n_left  = this->calculateItemPosition(resource_no - 1, resource_total);
        auto n_right = this->calculateItemPosition(resource_no + 1, resource_total);

        auto middle_item_offset = (n_right.x - (n_left.x + image_size.x) - image_size.x) / 2;
        position = n_left + sf::Vector2f(middle_item_offset, 0);
    }
    
    return position;
}