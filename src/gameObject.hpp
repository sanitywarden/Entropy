#ifndef _GAME_OBJECT_HPP_
#define _GAME_OBJECT_HPP_

#include <SFML/Graphics.hpp>

namespace iso {
    class GameObject : public sf::Drawable {
        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        public:
            sf::Vector2f object_position; // Object's position in the game world.
            sf::Vector2f object_size;     // Object's size, equivalent of the texture size.
            sf::Texture* object_texture;  // Object's texture.

        public:
            GameObject();
            GameObject(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, sf::Texture* texture);
            ~GameObject();
            
            bool exists() const;
    };
}

#endif