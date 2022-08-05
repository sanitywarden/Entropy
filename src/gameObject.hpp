#ifndef _GAME_OBJECT_HPP_
#define _GAME_OBJECT_HPP_

#include <SFML/Graphics.hpp>
#include <string>

namespace iso {
    class GameObject : public sf::Drawable {        
        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        public:
            std::string  object_name;         // Object's name indicates the object type -> "Building" or "Region" or "Tile".
            sf::Vector3f object_position;     // Object's position in the game world.
            sf::Vector2f object_size;         // Object's size, equivalent of the texture size.
            std::string  object_texture_name; // Object's texture name.
            sf::Color    object_colour;

        public:
            GameObject();
            GameObject(const GameObject& gameobject);
            GameObject(sf::Vector3f position, sf::Vector3f relative_position, sf::Vector2f size, const std::string& texture_name);
            GameObject(sf::Vector3f position, sf::Vector3f relative_position, sf::Vector2f size, const std::string& texture_name, const std::string& object_name);
            ~GameObject();
            
            bool exists() const;
            bool contains(sf::Vector2f point) const;

            std::string  getName()              const;
            sf::Vector3f getPosition()          const;
            sf::Vector2f getPosition2D()        const;
            sf::Vector2f getSize()              const;
            std::string  getTextureName()       const;
            sf::Color    getColour()            const;
    };
}

#endif