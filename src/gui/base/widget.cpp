#include <iostream>

#include "widget.hpp"

using namespace gui;
using namespace iso;

Widget::Widget() {
    this->manager = nullptr;
    std::cout << "[GUI][Widget]: Simulation manager is a nullptr.\n";
}

Widget::Widget(SimulationManager* manager, int x, int y) {
    this->manager      = manager;
    this->m_dimensions = sf::Vector2i(x, y); 
    this->texture_id   = "widget_base"; 
    this->m_blocks.resize(x * y);

    this->setWidgetSize(x * 64, y * 64);
}

Widget::Widget(SimulationManager* manager, sf::Vector2i dimensions) {
    this->manager      = manager;
    this->m_dimensions = dimensions;
    this->texture_id   = "widget_base"; 
    this->m_blocks.resize(dimensions.x * dimensions.y);

    this->setWidgetSize(dimensions.x * 64, dimensions.y * 64);
}

Widget::~Widget() {

}

void Widget::setWidgetTexture(std::string widget_texture_base) {
    this->texture_id = widget_texture_base;
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->show)
        return;

    auto widget_position = this->getWidgetPosition();
    
    // Draw the blocks that make up the foundation of the interface.
    // These are not custom elements such as text or images.
    for(int x = 0; x < this->m_dimensions.x; x++) {
        for(int y = 0; y < this->m_dimensions.y; y++) {
            const int index         = y * this->m_dimensions.x + x;
            Block block             = this->m_blocks.at(index);

            block.block_size         = sf::Vector2f(64, 64);
            block.block_texture_name = this->getBlockTexture(x, y, this->m_dimensions);

            const auto block_offset  = sf::Vector2f(x * block.block_size.x, y * block.block_size.y);
            block.block_position     = widget_position + block_offset;

            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(block.block_texture_name);
            target.draw(block, states);
        }
    }

    for(const auto& pair : this->m_components) {
        auto* component = pair.second;
        if(component->show)
            target.draw(*component);
    }
}

std::string Widget::getBlockTexture(int x, int y, sf::Vector2i dimensions) const {
    // Big widget.
    if(dimensions.x > 1 && dimensions.y > 1) {
        if(x == 0 && y == 0)
            return this->texture_id + "_top_left";

        else if(x == 0 && y == dimensions.y - 1)
            return this->texture_id + "_bottom_left";
        
        else if(x == dimensions.x - 1 && y == 0)
            return this->texture_id + "_top_right";
        
        else if(x == dimensions.x - 1 && y == dimensions.y - 1)
            return this->texture_id + "_bottom_right";

        else if(x != 0 && x != dimensions.x - 1 && y == 0)
            return this->texture_id + "_top";

        else if(x != 0 && x != dimensions.x - 1 && y == dimensions.y - 1)
            return this->texture_id + "_bottom";

        else if(y != 0 && y != dimensions.y - 1 && x == 0)
            return this->texture_id + "_left";
        
        else if(y != 0 && y != dimensions.y - 1 && x == dimensions.x - 1)
            return this->texture_id + "_right";

        else return this->texture_id + "_middle";
    }

    // Thin widget, vertical.
    else if(dimensions.x == 1 && dimensions.y != 1) {
        if(y == 0)
            return this->texture_id + "_small_vertical_top";

        else if(y == dimensions.y - 1)
            return this->texture_id + "_small_vertical_bottom";

        else return this->texture_id + "_small_vertical_middle";
    }

    // Thin widget, horizontal.
    else if(dimensions.y == 1 && dimensions.x != 1) {
        if(x == 0)
            return this->texture_id + "_small_horizontal_left";

        else if(x == dimensions.x - 1)
            return this->texture_id + "_small_horizontal_right";

        else return this->texture_id + "_small_horizontal_middle";
    }

    return this->texture_id + "_single";
} 

void Widget::addComponent(AbstractWidget* component, std::string id) {
    std::pair <std::string, gui::AbstractWidget*> pair = { id, component };
    component->setParent(this);
    
    component->setWidgetPosition(component->getWidgetPosition() + this->getWidgetPosition());
    
    this->m_components.insert(pair);
}

AbstractWidget* Widget::getComponentByName(std::string id) {
    return this->m_components.count(id)
        ? this->m_components[id]
        : nullptr;
}
