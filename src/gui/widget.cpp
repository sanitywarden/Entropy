#include "widget.hpp"
#include "simulationManager.hpp"

namespace gui {
Widget::Widget(const WidgetData& data, core::Vector2i dimensions, const std::string& texture) 
    : AbstractWidget(data), PropertyTileable(texture, dimensions)
{}

Widget::~Widget()
{}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    auto widget_position = this->getWidgetPosition();
    auto block_size = game_manager.resource.getTextureSize(this->base_texture + "_single").x;

    for(int x = 0; x < this->getDimensions().x; x++) {
        for(int y = 0; y < this->getDimensions().y; y++) {
            auto index = y * this->getDimensions().x + x;
            auto block = this->tiles.at(index);

            block.size = core::Vector2i(block_size, block_size);
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
}

std::string Widget::getBlockTexture(int x, int y) const {
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

void Widget::addComponent(AbstractComponent component) {
    auto id = component.get()->getWidgetID();
    this->data.components[id] = component;
}

AbstractWidget* Widget::getComponentByName(const std::string& id) {
    return this->data.components.count(id)
        ? this->data.components.at(id).get()
        : nullptr;
}
}