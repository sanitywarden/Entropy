#include "widgetNameCity.hpp"
#include "gamestate.hpp"
#include "nameGenerator.hpp"

using namespace gui;
using namespace iso;

WidgetSettleCity::WidgetSettleCity()
    : InterfacePage(nullptr) 
{}

WidgetSettleCity::WidgetSettleCity(SimulationManager* manager)
    : InterfacePage(manager)
{
    this->setWidgetID("component_widget_settle_city");
    this->createUI();

    this->region_id = -1;
    this->show = false;
}

WidgetSettleCity::~WidgetSettleCity() {

}

void WidgetSettleCity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;
    
    if(!this->show)
        return;
    
    auto* widget = static_cast<Widget*>(this->getComponent("widget_body"));
    if(widget)
        target.draw(*widget);

    auto* input_box = static_cast<InputBox*>(this->getComponent("input_box"));
    if(input_box)
        target.draw(*input_box);
}

void WidgetSettleCity::createUI() {
    auto window_size = this->manager->window.windowSize();

    sf::Vector2i t_widget_size(
        window_size.x / 300,
        window_size.y / 300
    );

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        sf::Vector2f widget_size = widget_body.get()->getWidgetSize();
        widget_body.get()->setWidgetID("widget_body"); 
        widget_body.get()->setWidgetPosition(sf::Vector2f(window_size.x / 2, window_size.y / 2) - sf::Vector2f(widget_size.x / 2, widget_size.y / 2));
        sf::Vector2f widget_position = widget_body.get()->getWidgetPosition();

    auto input_box = InputBoxComponent(new InputBox(this->manager, sf::Vector2i(t_widget_size.x * 5, t_widget_size.y * 2)));
        sf::Vector2f input_box_size = input_box.get()->getWidgetSize();
        input_box.get()->setWidgetID("input_box");
        input_box.get()->setWidgetPosition(widget_position + sf::Vector2f(0, widget_size.y) + sf::Vector2f(0, -input_box_size.y));
        sf::Vector2f input_box_position = input_box.get()->getWidgetPosition();
        input_box.get()->setText(generate(GenerationType::CITY, 1));

    this->addComponent(widget_body);
    this->addComponent(input_box);
}

void WidgetSettleCity::updateUI() { 
    // Key binds should be disabled while writing - 'R' should not generate the world while writing.
}

void WidgetSettleCity::functionality() {
    auto* input_box = static_cast<InputBox*>(this->getComponent("input_box"));
    auto* gamestate = this->manager->gamestate.getGamestate();
    
    if(input_box->containsPoint(gamestate->mouse_position_interface) && gamestate->controls.mouseLeftPressed()) {
        input_box->is_selected = true;
    }

    else if(!input_box->containsPoint(gamestate->mouse_position_interface) && gamestate->controls.mouseLeftPressed()) {
        input_box->is_selected = false;
    }
}   

void WidgetSettleCity::updateInput(std::string text) {
    auto* input_box = static_cast<InputBox*>(this->getComponent("input_box"));
    if(!input_box->is_selected)
        return;

    const int ascii_backspace = 8;
    const int ascii_enter     = 13;
    const int character_as_ascii_no = (int)(text[0]);
    if(character_as_ascii_no == ascii_backspace)
        input_box->shortenText(1);
    
    else if(character_as_ascii_no == ascii_enter) {
        auto city_name = input_box->getText();
        
        auto& region = this->manager->world.world_map[this->region_id];
        region.settlement_name = city_name;
        input_box->setText("");

        this->region_id = -1;
        this->show      = false;
        input_box->is_selected = false;
        
        auto* gamestate = this->manager->gamestate.getGamestate();
        gamestate->block_keybinds = false;

        return;
    }

    else
        input_box->appendText(text);
}

bool WidgetSettleCity::acceptsInput() {
    auto* input_box = static_cast<InputBox*>(this->getComponent("input_box"));
    return input_box->is_selected;
}