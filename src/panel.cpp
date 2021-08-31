#include "panel.hpp"

namespace iso {
    Panel::Panel() {
        this->is_river   = false;
        this->is_arctic  = false;
        this->is_coast   = false;
        this->is_terrain = false;
        this->marked     = false;
        this->latitude = 0.0f;
    }

    Panel::~Panel() {}

    void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // Create the vertex array with intent of creating a square (rectangle).
        sf::VertexArray panel(sf::Quads, 4);

        // Create a sqaure out of four points.
        panel[0].position = this->panel_position;
        panel[1].position = this->panel_position + sf::Vector2f(this->panel_size.x, 0);
        panel[2].position = this->panel_position + sf::Vector2f(this->panel_size.x, this->panel_size.y);
        panel[3].position = this->panel_position + sf::Vector2f(0, this->panel_size.y);
        
        panel[0].texCoords = sf::Vector2f(0, 0);
        panel[1].texCoords = sf::Vector2f(this->panel_size.x, 0);
        panel[2].texCoords = sf::Vector2f(this->panel_size.x, this->panel_size.y);
        panel[3].texCoords = sf::Vector2f(0, this->panel_size.y);

        // Do not delete!
        // If you do not remember what it does, uncomment, and see once again.
        // panel[0].color = sf::Color(this->noise_value * 255, this->noise_value * 255, this->noise_value * 255);
        // panel[1].color = sf::Color(this->noise_value * 255, this->noise_value * 255, this->noise_value * 255);
        // panel[2].color = sf::Color(this->noise_value * 255, this->noise_value * 255, this->noise_value * 255);
        // panel[3].color = sf::Color(this->noise_value * 255, this->noise_value * 255, this->noise_value * 255);

        // Draw the panel.
        states.texture = &this->panel_texture;
        target.draw(panel, states);
        
        // And then draw all the present features.
        // Draw the river texture.
        if(this->feature.river.texture != nullptr) {
            states.texture = this->feature.river.texture;
            target.draw(panel, states);
        }
    }
}