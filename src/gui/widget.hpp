#pragma once

#include "abstractWidget.hpp"
#include "tileable.hpp"

#include <map>

namespace gui {
class Widget : public AbstractWidget, public PropertyTileable {
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::string getBlockTexture(int x, int y) const override;
    public:
        Widget(const WidgetData& data, core::Vector2i dimensions, const std::string& texture);
        ~Widget();

        void addComponent(AbstractComponent component);
        AbstractWidget* getComponentByName(const std::string& id);
};

typedef std::shared_ptr <Widget> WidgetComponent;
}