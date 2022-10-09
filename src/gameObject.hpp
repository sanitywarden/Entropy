#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "types.hpp"

namespace iso {
    class GameObject : public sf::Drawable {        
        private:
            static int instance_id;

        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        public:
            std::string    object_name;         // Object's name indicates the object type -> "Building" or "Region" or "Tile".
            core::Vector3f object_position;     // Object's position in the game world.
            core::Vector2i object_size;         // Object's size, equivalent of the texture size.
            std::string    object_texture_name; // Object's texture name.
            core::Colour   object_colour;

        public:
            GameObject();
            GameObject(const GameObject& object);
            GameObject(core::Vector3f position, core::Vector3f relative_position, core::Vector2i size, const std::string& texture_name);
            GameObject(core::Vector3f position, core::Vector3f relative_position, core::Vector2i size, const std::string& texture_name, const std::string& object_name);
            ~GameObject();
            
            bool exists() const;
            bool hasName() const;
            bool contains(core::Vector2f point) const;

            const std::string& getName()        const;
            core::Vector3f     getPosition()    const;
            core::Vector2f     getPosition2D()  const;
            core::Vector2i     getSize()        const;
            const std::string& getTextureName() const;
            core::Colour       getColour()      const;
            int                getInstanceId()  const;
    };
}