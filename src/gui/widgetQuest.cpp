#include "widgetQuest.hpp"

using namespace gui;
using namespace iso;

WidgetQuest::WidgetQuest(SimulationManager* manager) 
    : InterfacePage(manager)
{
    this->setWidgetID("component_widget_quest");
    this->createUI();
}

WidgetQuest::~WidgetQuest() {

}

void WidgetQuest::createUI() {
    // Create UI for the banners that pop up on the top of the screen.
    // Create UI for the widget that pops up upon pressing a quest banner on the top. 

    auto window_size = this->manager->window.windowSize();

    auto t_widget_size = sf::Vector2i(
        window_size.x / 200,
        window_size.y / 400
    );

    auto widget_body = WidgetComponent(new Widget(this->manager, t_widget_size));
        auto widget_size = widget_body.get()->getWidgetSize();
        widget_body.get()->setWidgetID("widget_body");
        widget_body.get()->setWidgetPosition(sf::Vector2f(window_size.x / 2 - widget_size.x / 2, window_size.y / 2 - widget_size.y / 2));
        auto widget_position = widget_body.get()->getWidgetPosition();

    auto widget_title = ButtonComponent(new Button(this->manager, sf::Vector2i(t_widget_size.x * 2, t_widget_size.y * 2), ""));
        auto widget_title_size = widget_title.get()->getWidgetSize();
        widget_title.get()->setWidgetID("widget_body_title");
        widget_title.get()->setWidgetPosition(sf::Vector2f(window_size.x / 2 - widget_title_size.x / 2, window_size.y / 2 - widget_size.y / 2 - widget_title_size.y));
        auto widget_title_position = widget_title.get()->getWidgetPosition();

    auto quest_title = LabelComponent(new Label(this->manager, "Title"));
        quest_title.get()->setWidgetID("widget_title");
        quest_title.get()->align(Alignment::ALIGMNENT_CENTRED_TOP, widget_title.get());

    auto quest_description = LabelComponent(new Label(this->manager, "Description"));
        quest_description.get()->setWidgetID("widget_description");
        quest_description.get()->setWidgetPosition(sf::Vector2f(widget_position.x + 8, widget_position.y + 8));

    this->addComponent(widget_body);
    this->addComponent(widget_title);
    this->addComponent(quest_title);
    this->addComponent(quest_description);
}

void WidgetQuest::updateUI() {
    // Update the text displayed.
}

void WidgetQuest::functionality() {
    // Maybe buttons?
}

void WidgetQuest::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;
    
    if(!this->show)
        return;

    auto widget_body = static_cast<Widget*>(this->getComponent("widget_body"));
    if(widget_body)
        this->manager->window.draw(*widget_body);

    auto widget_body_title = static_cast<Widget*>(this->getComponent("widget_body_title"));
    if(widget_body_title)
        this->manager->window.draw(*widget_body_title);

    auto quest_title = static_cast<Label*>(this->getComponent("widget_title"));
    if(quest_title)
        this->manager->window.draw(*quest_title);

    auto quest_description = static_cast<Label*>(this->getComponent("widget_description"));
    if(quest_description)
        this->manager->window.draw(*quest_description);
} 
