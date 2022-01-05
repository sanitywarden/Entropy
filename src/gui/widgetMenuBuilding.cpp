#include "widgetMenuBuilding.hpp"
#include "regionmap.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

WidgetMenuBuilding::WidgetMenuBuilding() {
    this->manager = nullptr;
    std::cout << "[GUI][WidgetMenuBuilding]: Simulation manager is a nullptr.\n";
}

WidgetMenuBuilding::WidgetMenuBuilding(SimulationManager* manager) : InterfacePage(manager) {
    this->setWidgetID("component_widget_menu_building");
    this->createUI();

    this->last_selected_building = "building_empty";
}

WidgetMenuBuilding::~WidgetMenuBuilding() {

}

void WidgetMenuBuilding::createUI() {
    sf::Vector2i t_widget_size(10, 3);
    int window_width  = this->manager->window.windowWidth();
    int window_height = this->manager->window.windowHeight();

    static Widget widget_body(this->manager, t_widget_size);
        sf::Vector2f widget_size = widget_body.getWidgetSize();
        widget_body.setWidgetID("widget_menu_building");
        widget_body.setWidgetPosition(window_width / 2 - widget_size.x / 2, window_height - widget_size.y);
        sf::Vector2f widget_position = widget_body.getWidgetPosition();

    static ImageHolder image_building_house(this->manager, "building_primitive_house");
        image_building_house.setWidgetID("imageholder_building_house");
        image_building_house.setWidgetSize(64, 64);
        image_building_house.setWidgetPosition(widget_position);

    static ImageHolder image_building_quarry(this->manager, "building_quarry");
        image_building_quarry.setWidgetID("imageholder_building_quarry");
        image_building_quarry.setWidgetSize(64, 64);
        image_building_quarry.setWidgetPosition(widget_position + sf::Vector2f(64, 0));

    static ImageHolder image_building_farm(this->manager, "building_farmland");
        image_building_farm.setWidgetID("imageholder_building_farm");
        image_building_farm.setWidgetSize(64, 64);
        image_building_farm.setWidgetPosition(widget_position + sf::Vector2f(128, 0));

    static ImageHolder image_building_woodcutter(this->manager, "building_woodcutter");
        image_building_woodcutter.setWidgetID("imageholder_building_woodcutter");
        image_building_woodcutter.setWidgetSize(64, 64);
        image_building_woodcutter.setWidgetPosition(widget_position + sf::Vector2f(192, 0));

    this->interface.insert({ widget_body.getWidgetID()              , &widget_body               });
    this->interface.insert({ image_building_house.getWidgetID()     , &image_building_house      });
    this->interface.insert({ image_building_farm.getWidgetID()      , &image_building_farm       });
    this->interface.insert({ image_building_quarry.getWidgetID()    , &image_building_quarry     });
    this->interface.insert({ image_building_woodcutter.getWidgetID(), &image_building_woodcutter });
}

void WidgetMenuBuilding::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->manager == nullptr)
        return;

    if(!this->show)
        return;

    auto* widget = static_cast<Widget*>(this->interface.at("widget_menu_building"));
    if(widget)
        target.draw(*widget);

    auto* image1 = static_cast<ImageHolder*>(this->interface.at("imageholder_building_house"));
    if(image1)
        target.draw(*image1);

    auto* image2 = static_cast<ImageHolder*>(this->interface.at("imageholder_building_farm"));
    if(image2)
        target.draw(*image2);

    auto* image3 = static_cast<ImageHolder*>(this->interface.at("imageholder_building_quarry"));
    if(image3)
        target.draw(*image3);

    auto* image4 = static_cast<ImageHolder*>(this->interface.at("imageholder_building_woodcutter"));
    if(image4)
        target.draw(*image4);
}

/* Returns the texture name of the image under mouse position. 
 * Returns "building_empty" if no image is there. */
void WidgetMenuBuilding::findBuilding() {
    auto* gamestate = this->manager->gamestate.getGamestate(); 
    auto* regionmap = static_cast<Regionmap*>(gamestate);
    auto mouse_position = regionmap->mouse_position_interface;

    for(const auto& pair : this->interface) {
        auto* component = pair.second;
        if(component) {
            if(component->getWidgetID() != "widget_menu_building") {
                if(component->containsPoint(mouse_position) && gamestate->controls.mouseLeftPressed()) {
                    auto* image_holder = static_cast<ImageHolder*>(component);
                    auto  texture_name = image_holder->getTextureName();
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
    for(int index = 0; index < BUILDING_LOOKUP_TABLE.size(); index++) {
        auto building = BUILDING_LOOKUP_TABLE[index];

        if(building_name == building.getName())
            return building;
    }
}