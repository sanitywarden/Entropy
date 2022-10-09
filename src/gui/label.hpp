#pragma once

#include "abstractWidget.hpp"

namespace gui {
enum class Alignment {
    ALIGNMENT_CENTRED,
    ALIGNMENT_LEFT,
    ALIGNMENT_RIGHT,
    ALIGNMENT_TOP,
    ALIGNMENT_BOTTOM
};

class Label : public AbstractWidget {
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        Alignment alignment_x;
        Alignment alignment_y;
        std::string text;
        core::Colour colour;
    public:
        Label(const WidgetData& data, std::string text, Alignment x, Alignment y);
        ~Label();

        void setColour(core::Colour colour);
        void setString(const std::string& text);
};

typedef std::shared_ptr <Label>  LabelComponent;
}