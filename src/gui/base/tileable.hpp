#ifndef _GUI_PROPERTY_BLOCK_HPP_
#define _GUI_PROPERTY_BLOCK_HPP_

#include <SFML/Graphics.hpp>

namespace gui {
    class Block : public sf::Drawable {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        
        public:
            std::string  block_texture_name;
            sf::Vector2f block_position;
            sf::Vector2f block_size;

        public:
            Block();
            ~Block();
    };

    /*  This class is a Utility for creating fast, tileable UI. */
    class PropertyTileable {
        protected:
            sf::Vector2i        m_dimensions;
            std::vector <Block> m_blocks;

        public:
            PropertyTileable();
            ~PropertyTileable();

            virtual std::string getBlockTexture(int current_x, int current_y, sf::Vector2i dimensions) const;
            sf::Vector2i getBlockSize() const;
            void setBlockSize(sf::Vector2i size);
    };
}

#endif