#include "widgetMainMenu.hpp"
#include "globalutilities.hpp"
#include "menu.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

WidgetMainMenu::WidgetMainMenu() 
    : InterfacePage(nullptr)    
{

}

WidgetMainMenu::WidgetMainMenu(SimulationManager* manager) 
    : InterfacePage(manager)
{
    this->setWidgetID("component_main_menu");
    this->createUI();
}

WidgetMainMenu::~WidgetMainMenu() {

}

void WidgetMainMenu::createUI() {
    int size_x = this->manager->window.windowWidth()  / 300;
    int size_y = this->manager->window.windowHeight() / 80;

    sf::Vector2i t_widget_size(size_x, size_y); 
    sf::Vector2f window_size = this->manager->window.windowSize();
    int window_width  = this->manager->window.windowWidth();
    int window_height = this->manager->window.windowHeight(); 

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        sf::Vector2f widget_size = widget_body.get()->getWidgetSize();
        widget_body.get()->setWidgetID("widget_body");
        widget_body.get()->setWidgetPosition(sf::Vector2f(window_width * 0.15f, 0));
        sf::Vector2f widget_position = widget_body.get()->getWidgetPosition();
    
    sf::Vector2i t_button_size(size_x * 4, size_x);
    auto button_start = ButtonComponent(new Button(this->manager, t_button_size, "Play"));
        sf::Vector2f button_size = button_start.get()->getWidgetSize();
        sf::Vector2f button_offset_within = sf::Vector2f((widget_size.x - button_size.x) / 2, widget_size.y * 0.15f);
        sf::Vector2f first_button_position = widget_position + button_offset_within;
        button_start.get()->setWidgetID("button_start_game");
        button_start.get()->setWidgetPosition(first_button_position);
        sf::Vector2f button_position = button_start.get()->getWidgetPosition();
        button_start.get()->label.setWidgetPosition(button_position + sf::Vector2f(button_size.x / 2, button_size.y / 2));

    auto button_exit = ButtonComponent(new Button(this->manager, t_button_size, "Exit"));
        sf::Vector2f button_offset = sf::Vector2f(0, button_size.y / 2);
        button_exit.get()->setWidgetID("button_exit_game");
        button_exit.get()->setWidgetPosition(first_button_position + sf::Vector2f(0, button_size.y) + button_offset);
        button_exit.get()->label.setWidgetPosition(button_exit.get()->getWidgetPosition() + sf::Vector2f(button_size.x / 2, button_size.y / 2));    

    this->addComponent(widget_body);
    this->addComponent(button_start);
    this->addComponent(button_exit);
}

void WidgetMainMenu::updateUI() {

}

void WidgetMainMenu::functionality() {
    auto* menu = static_cast<Menu*>(this->manager->gamestate.getGamestate());
    auto* button_start = static_cast<Button*>(this->getComponent("button_start_game"));

    if(menu->controls.mouseLeftPressed() && button_start->containsPoint(menu->mouse_position_interface)) {
        this->manager->gamestate.setGamestate("worldmap");
    }

    auto* button_exit = static_cast<Button*>(this->getComponent("button_exit_game"));
    if(menu->controls.mouseLeftPressed() && button_exit->containsPoint(menu->mouse_position_interface)) {
        this->manager->exitApplication(0);
    }
}

void WidgetMainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;
    
    auto* widget = static_cast<Widget*>(this->getComponent("widget_body"));
    if(widget)
        this->manager->window.draw(*widget);
    
    for(const auto& pair : this->interface) {
        auto component = pair.second;

        if(startsWith(component.get()->getWidgetID(), "button"))
            this->manager->window.draw(*component);
    }
}