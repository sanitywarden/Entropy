#pragma once

#include "abstractWidget.hpp"

namespace gui {
class TileableBlock : public sf::Drawable {
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates) const override;
    
    public:
        std::string texture;
        core::Vector2i position;
        core::Vector2i size;
};

class PropertyTileable : sf::Drawable {
    protected:
        std::string    base_texture;
        core::Vector2i dimensions;
        std::vector <TileableBlock> tiles;
    
    public:
        PropertyTileable(const std::string& base_texture, core::Vector2i dimensions);
        ~PropertyTileable();
    
        virtual std::string getBlockTexture(int current_x, int current_y) const;
        core::Vector2i getDimensions() const;
    };
}