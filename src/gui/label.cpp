#include "label.hpp"
#include "simulationManager.hpp"
#include "guiutilities.hpp"
#include "globalutilities.hpp"

namespace gui {
Label::Label(const WidgetData& data, std::string text, Alignment x, Alignment y)
    : AbstractWidget(data), text(text), alignment_x(x), alignment_y(y), font_name("garamond")
{}

Label::~Label()
{}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(this->text == "")
        return;

    auto parent_position = core::Vector2i(0, 0);
    auto parent_size     = core::Vector2i(0, 0);
    
    if(this->hasParent()) {
        parent_position = this->getParent()->getWidgetPosition();
        parent_size     = this->getParent()->getWidgetSize();
    }

    sf::Text text(this->text, game_manager.resource.getFont("garamond"), game_manager.font_size);

    auto rect_local     = text.getLocalBounds();
    auto final_origin   = core::Vector2i(0, 0);
    auto final_position = core::Vector2i(0, 0);

    auto align = [&](Alignment alignment, bool is_x) {
        switch(alignment) {
            default: {
                iso::printError("Label::draw()", "Alignment option not implemented (Alignment::" + std::to_string((int)alignment) + ")");        
                break;
            }

            case Alignment::ALIGNMENT_CENTRED: {
                if(is_x) {
                    final_origin.x   = rect_local.left + rect_local.width / 2;
                    final_position.x = parent_position.x + parent_size.x / 2;
                }
                
                else {
                    final_origin.y   = rect_local.top + rect_local.height / 2;
                    final_position.y = parent_position.y + parent_size.y / 2; 
                }
                
                break;
            }
        }
    };

    align(this->alignment_x, true);
    align(this->alignment_y, false);

    text.setPosition(final_position.asSFMLVector2f());
    text.setOrigin(final_origin.asSFMLVector2f());
    text.setFillColor(this->colour.asSFMLColour());
    text.setFont(game_manager.resource.getFont(this->font_name));

    target.draw(text, states);
} 

void Label::setColour(core::Colour colour) {
    this->colour = colour;
}

void Label::setString(const std::string& text) {
    this->text = text;
}

void Label::setFont(const std::string& font_name) {
    this->font_name = font_name;
}
}