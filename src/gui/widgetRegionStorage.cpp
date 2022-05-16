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
    sf::Vector2i t_widget_size(6, 10);
    int window_width  = this->manager->window.windowWidth();
    int window_height = this->manager->window.windowHeight();

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        sf::Vector2f widget_size = widget_body.get()->getWidgetSize();
        widget_body.get()->setWidgetID("widget_menu_items");
        widget_body.get()->setWidgetPosition(window_width - widget_size.x, window_height - widget_size.y);
        sf::Vector2f widget_position = widget_body.get()->getWidgetPosition();

    this->addComponent(widget_body);
}

void WidgetRegionStorage::updateUI() {
    if(this->should_refresh) {
        this->refresh();
        this->should_refresh = false;
    }
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

void WidgetRegionStorage::refresh() {
    for(auto pair : this->interface) {
        auto component = pair.second;
        auto component_name = component.get()->getWidgetID();

        if(startsWith(component_name, "imageholder_"))
            this->deleteComponent(component_name);

        if(startsWith(component_name, "text_quantity_"))
            this->deleteComponent(component_name);
    }

    auto image_size = sf::Vector2f(48, 48);
    auto offset     = sf::Vector2f(image_size.x / 2, image_size.y / 2);
    int item_no = 0;
    
    auto widget_position = this->getComponent("widget_menu_items")->getWidgetPosition();
    auto widget_size     = this->getComponent("widget_menu_items")->getWidgetSize();

    // Calculate the number of icons possible to fit in this widget.

    int row_size = 0;
    for(const auto& building : RESOURCE_LOOKUP_TABLE) {
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

    auto* gamestate = this->manager->gamestate.getGamestate();
    auto* regionmap = static_cast<Regionmap*>(gamestate);
    auto* current_region = regionmap->getCurrentRegion();
    for(const auto& resource : RESOURCE_LOOKUP_TABLE) {
        if(current_region->resources.count(resource.getName())) {
            auto image = ImageComponent(new ImageHolder(this->manager, resource.getIcon()));
            auto name_image = "imageholder_" + toLower(resource.getName());
            image.get()->setWidgetID(name_image);
            
            auto text = LabelComponent(new Label(this->manager, std::to_string(current_region->getResourceQuantity(resource))));
            auto name_text = "text_quantity_" + toLower(resource.getName());
            text.get()->setWidgetID(name_text);

            // std::cout << resource.getName() << " quantity: " << resource.getQuantity() << "\n";

            const int x = item_no % row_size;
            const int y = item_no / row_size;

            // Image's final position.
            auto final_position = widget_position + offset + sf::Vector2f(x * image_size.x, y * image_size.y) + sf::Vector2f(x * offset.x, 0);

            image.get()->setWidgetPosition(final_position);
            image.get()->setWidgetSize(image_size);

            // Quantity text's final position
            auto text_final_position = final_position + sf::Vector2f(0, 32) + sf::Vector2f(4, -4); 

            text.get()->setWidgetPosition(text_final_position);

            this->addComponent(image);
            this->addComponent(text);

            item_no++;
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