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

static std::map <std::string, Alignment> ALIGNMENT_ID_TABLE = {
    { "Centred", Alignment::ALIGNMENT_CENTRED },
    { "Left"   , Alignment::ALIGNMENT_LEFT    },
    { "Right"  , Alignment::ALIGNMENT_RIGHT   },
    { "Top"    , Alignment::ALIGNMENT_TOP     },
    { "Bottom" , Alignment::ALIGNMENT_BOTTOM  },
};

class Label : public AbstractWidget {
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        Alignment alignment_x;
        Alignment alignment_y;
        std::string text;
        std::string font_name;
        core::Colour colour;
    public:
        Label(const WidgetData& data, std::string text, Alignment x, Alignment y);
        ~Label();

        void setString(const std::string& text);
        const std::string& getString() const;

        void setColour(core::Colour colour);
        core::Colour getColour() const;

        void setFont(const std::string& font);
        const std::string& getFont() const;
};

typedef std::shared_ptr <Label> LabelComponent;
}