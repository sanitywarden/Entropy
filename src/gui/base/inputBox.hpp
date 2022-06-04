#ifndef _GUI_INPUT_BOX_HPP_
#define _GUI_INPUT_BOX_HPP_

#include "abstractWidget.hpp"
#include "tileable.hpp"
#include "simulationManager.hpp"
#include "label.hpp"

namespace gui {
    class InputBox : public AbstractWidget, public PropertyTileable {
        private:
            iso::SimulationManager* manager;
            std::string current_text;        // Current text received from input.
            std::string texture_id;
            int input_length_limit;          // How many characters can you type in.
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            std::string getBlockTexture(int x, int y, sf::Vector2i dimensions) const override;
        public:
            InputBox();
            InputBox(iso::SimulationManager* manager, sf::Vector2i dimensions);
            ~InputBox();

            void setText(std::string input);
            void appendText(std::string input);
            void shortenText(int shorten_by = 1);
            std::string getText() const;

            bool is_selected; // If the input box should listen for keystrokes.
            Label label;      // This label displays data entered by the user.
    };
}

#endif