#ifndef _GAME_OBJECT_HPP_
#define _GAME_OBJECT_HPP_

#include <SFML/Graphics.hpp>
#include <string>

namespace iso {
    class GameObject : public sf::Drawable {        
        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        public:
            std::string  object_name;         // Object's name indicates the type and purpose -> "building_house".
            sf::Vector2f object_position;     // Object's position in the game world.
            sf::Vector2f object_size;         // Object's size, equivalent of the texture size.
            std::string  object_texture_name; // Object's texture name.
            sf::Color    object_colour;

        public:
            GameObject();
            GameObject(const GameObject& gameobject);
            GameObject(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name);
            ~GameObject();
            
            bool exists() const;
            bool contains(sf::Vector2f point) const;

            std::string  getName()        const; 
            sf::Vector2f getPosition()    const;
            sf::Vector2f getSize()        const;
            std::string  getTextureName() const;
            sf::Color    getColour()      const;
    };
}

#endif