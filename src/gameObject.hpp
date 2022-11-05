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
            core::Vector3i object_position;     // Object's position in the game world.
            core::Vector2i object_size;         // Object's size, equivalent of the texture size.
            std::string    object_texture_name; // Object's texture name.
            core::Colour   object_colour;
            int            index;

        public:
            GameObject();
            GameObject(const GameObject& object);
            GameObject(core::Vector3i position, core::Vector3i relative_position, core::Vector2i size, const std::string& texture_name);
            GameObject(core::Vector3i position, core::Vector3i relative_position, core::Vector2i size, const std::string& texture_name, const std::string& object_name);
            GameObject(core::Vector3i position, core::Vector2i size, const std::string& texture_name);
            ~GameObject();
            
            bool exists() const;
            bool hasName() const;
            bool contains(core::Vector2i point) const;

            const std::string& getName()        const;
            core::Vector3i     getPosition()    const;
            core::Vector2i     getPosition2D()  const;
            core::Vector2i     getSize()        const;
            const std::string& getTextureName() const;
            core::Colour       getColour()      const;
            int                getInstanceId()  const;
            int                getIndex()       const;

            void setName(const std::string& name);
            void setPosition(core::Vector3i position);
            void setSize(core::Vector2i size);
            void setTextureName(const std::string& texture_name);
            void setColour(core::Colour colour);
    };
}