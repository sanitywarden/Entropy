#pragma once

#include "abstractWidget.hpp"
#include "label.hpp"
#include "tileable.hpp"

namespace gui {
class Button : public AbstractWidget, public PropertyTileable {
    private:
        LabelComponent label;
    
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::string getBlockTexture(int x, int y) const override;
    public:
        Button(const WidgetData& data, core::Vector2i dimensions, const std::string& texture);
        ~Button();

        void addLabel(const WidgetData& label_data, const std::string& text);
};

typedef std::shared_ptr <Button> ButtonComponent;
}