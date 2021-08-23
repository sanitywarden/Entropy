#include "panel.hpp"

namespace iso {
    Panel::Panel() {
        this->is_arctic  = false;
        this->is_coast   = false;
        this->is_terrain = false;
        this->marked     = false;
        this->panel_feature_texture = nullptr;

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

        states.texture = &this->panel_texture;

        target.draw(panel, states);
        
        states.texture = this->panel_feature_texture;

        if(states.texture != nullptr) target.draw(panel, states);
    }
}