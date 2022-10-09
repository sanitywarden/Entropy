#include "button.hpp"
#include "simulationManager.hpp"

namespace gui {
Button::Button(const WidgetData& data, core::Vector2i dimensions, const std::string& texture)
    : AbstractWidget(data), PropertyTileable(texture, dimensions), label(nullptr)
{}

Button::~Button() 
{}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    auto widget_position = this->getWidgetPosition();

    for(int x = 0; x < this->getDimensions().x; x++) {
        for(int y = 0; y < this->getDimensions().y; y++) {
            auto index = y * this->getDimensions().x + x;
            auto block = this->tiles.at(index);

            block.size = core::Vector2i(8, 8);
            block.texture = this->getBlockTexture(x, y);

            auto offset = core::Vector2i(x * block.size.x, y * block.size.y);
            block.position = widget_position + offset;

            sf::RenderStates states;
            states.texture = &game_manager.resource.getTexture(block.texture);
            target.draw(block, states);
        }
    }

    for(const auto& pair : this->data.components) {
        auto* component = pair.second.get();
        if(component->isVisible())
            target.draw(*component);
    }

    if(label)
        target.draw(*this->label);
}

void Button::addLabel(const WidgetData& label_data, const std::string& text) {
    if(this->label != nullptr)
        return;

    auto label = LabelComponent(new Label(label_data, text, Alignment::ALIGNMENT_CENTRED, Alignment::ALIGNMENT_CENTRED));
    this->label = label;
}

std::string Button::getBlockTexture(int x, int y) const {
    auto dimensions = this->getDimensions();

    // Big widget.
    if(dimensions.x > 1 && dimensions.y > 1) {
        if(x == 0 && y == 0)
            return this->base_texture + "_top_left";

        else if(x == 0 && y == dimensions.y - 1)
            return this->base_texture + "_bottom_left";
        
        else if(x == dimensions.x - 1 && y == 0)
            return this->base_texture + "_top_right";
        
        else if(x == dimensions.x - 1 && y == dimensions.y - 1)
            return this->base_texture + "_bottom_right";

        else if(x != 0 && x != dimensions.x - 1 && y == 0)
            return this->base_texture + "_top";

        else if(x != 0 && x != dimensions.x - 1 && y == dimensions.y - 1)
            return this->base_texture + "_bottom";

        else if(y != 0 && y != dimensions.y - 1 && x == 0)
            return this->base_texture + "_left";
        
        else if(y != 0 && y != dimensions.y - 1 && x == dimensions.x - 1)
            return this->base_texture + "_right";

        else return this->base_texture + "_middle";
    }

    // Thin widget, vertical.
    else if(dimensions.x == 1 && dimensions.y != 1) {
        if(y == 0)
            return this->base_texture + "_small_vertical_top";

        else if(y == dimensions.y - 1)
            return this->base_texture + "_small_vertical_bottom";

        else return this->base_texture + "_small_vertical_middle";
    }

    // Thin widget, horizontal.
    else if(dimensions.y == 1 && dimensions.x != 1) {
        if(x == 0)
            return this->base_texture + "_small_horizontal_left";

        else if(x == dimensions.x - 1)
            return this->base_texture + "_small_horizontal_right";

        else return this->base_texture + "_small_horizontal_middle";
    }

    return this->base_texture + "_single";
}
}