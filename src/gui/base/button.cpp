#include "button.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

Button::Button() {
    this->manager = nullptr;
    std::cout << "[GUI][Button]: Simulation manager is a nullptr.\n";
}

Button::Button(SimulationManager* manager, sf::Vector2i dimensions, std::string data) : label(manager, data) {
    this->manager      = manager;
    this->m_dimensions = dimensions;
    this->m_blocks.resize(dimensions.x * dimensions.y);
    this->setWidgetSize(dimensions.x * 8, dimensions.y * 8);
    this->texture_id = "button_base";
}

Button::~Button() {
    
} 

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;

    if(!this->show)
        return;

    auto widget_position = this->getWidgetPosition();
    for(int x = 0; x < this->m_dimensions.x; x++) {
        for(int y = 0; y < this->m_dimensions.y; y++) {
            const int index = y * this->m_dimensions.x + x;
            Block block     = this->m_blocks.at(index);

            block.block_size         = sf::Vector2f(8, 8);
            block.block_texture_name = this->getBlockTexture(x, y, this->m_dimensions);
        
            const auto block_offset = sf::Vector2f(x * block.block_size.x, y * block.block_size.y);
            block.block_position    = widget_position + block_offset;

            sf::RenderStates states;
            states.texture = &this->manager->resource.getTexture(block.block_texture_name);
            target.draw(block, states);
        }
    }

    if(this->label.show) {
        this->label.align(Alignment::ALIGNMENT_CENTRED, widget_position, this->getWidgetSize());
        target.draw(this->label);
    }
}

std::string Button::getBlockTexture(int x, int y, sf::Vector2i dimensions) const {
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

void Button::setWidgetTexture(std::string texture_name_base) {
    this->texture_id = texture_name_base;
}
