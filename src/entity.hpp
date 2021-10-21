#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include <SFML/Graphics.hpp>

namespace iso {
    // Entity is a representation of a feature.
    // It is supposed to be attached to a pre-existing high-level structure, such as a Region or a Tile, thus it does not have position property of it's own.
    // It has a property which describes it's position relative to the item it is attached to.
    class Entity : public sf::Drawable {
        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

        public:
            Entity();
            Entity(sf::Vector2f parent_position, sf::Vector2f relative_position, sf::Vector2f size, sf::Texture* texture);
            ~Entity();

            sf::Vector2f parent_position;
            sf::Vector2f relative_position; // Relative position of the entity towards it's parent.
            sf::Texture* texture;
            sf::Vector2f size;

            bool exists() const;
    };
}

#endif