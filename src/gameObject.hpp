#ifndef _GAME_OBJECT_HPP_
#define _GAME_OBJECT_HPP_

#include <SFML/Graphics.hpp>
#include <string>

namespace iso {
    class GameObject : public sf::Drawable {
        friend class Building;
        
        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        protected:
            std::string  object_name; // Object's name. Not unique.

        public:
            sf::Vector2f object_position;     // Object's position in the game world.
            sf::Vector2f object_size;         // Object's size, equivalent of the texture size.
            std::string  object_texture_name; // Object's texture name.

        public:
            GameObject();
            GameObject(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name);
            ~GameObject();
            
            bool exists()                 const;
            std::string  getName()        const; 
            sf::Vector2f getPosition()    const;
            sf::Vector2f getSize()        const;
            std::string  getTextureName() const;
    };
}

#endif