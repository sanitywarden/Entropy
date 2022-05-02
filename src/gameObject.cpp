#include "gameObject.hpp"
#include "colours.hpp"

#include <iostream>

using namespace iso;

GameObject::GameObject() {
    this->object_name         = "*";
    this->object_position     = sf::Vector3f();
    this->object_size         = sf::Vector2f();
    this->object_texture_name = "*";
    this->object_colour       = COLOUR_BLACK;
}

GameObject::GameObject(const GameObject& gameobject) {
    this->object_name         = gameobject.getName();
    this->object_position     = gameobject.getPosition();
    this->object_size         = gameobject.getSize();
    this->object_texture_name = gameobject.getTextureName();
    this->object_colour       = gameobject.getColour();
}

GameObject::GameObject(sf::Vector3f position, sf::Vector3f relative_position, sf::Vector2f size, std::string texture_name) {
    this->object_name         = "*";
    this->object_position     = position + relative_position;
    this->object_size         = size;
    this->object_texture_name = texture_name;
    this->object_colour       = COLOUR_BLACK;
}

GameObject::GameObject(sf::Vector3f position, sf::Vector3f relative_position, sf::Vector2f size, std::string texture_name, std::string object_name) {
    this->object_name         = object_name;
    this->object_position     = position + relative_position;
    this->object_size         = size;
    this->object_texture_name = texture_name;
    this->object_colour       = COLOUR_BLACK;  
}

GameObject::~GameObject() {

}

bool GameObject::exists() const {
    return this->object_texture_name[0] != '*';
}

bool GameObject::contains(sf::Vector2f point) const {
    auto position2d = this->getPosition2D();
    sf::Rect object_rectangle(position2d, this->object_size);
    return object_rectangle.contains(point);
}

std::string GameObject::getName() const {
    return this->object_name;
}

sf::Vector3f GameObject::getPosition() const {
    return this->object_position;
}

// When you represent a 3D object as a 2D isometric object, you just subtract the Z axis from the Y axis.
sf::Vector2f GameObject::getPosition2D() const {
    return sf::Vector2f(this->object_position.x, this->object_position.y + this->object_position.z);
}

sf::Vector2f GameObject::getSize() const {
    return this->object_size;
}

std::string GameObject::getTextureName() const {
    return this->object_texture_name;
}

sf::Color GameObject::getColour() const {
    return this->object_colour;
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray game_object(sf::Quads, 4);

    auto position2d = this->getPosition2D();
    auto size       = this->getSize();
    auto colour     = this->getColour();

    game_object[0].position = position2d;
    game_object[1].position = position2d + sf::Vector2f(size.x, 0);
    game_object[2].position = position2d + sf::Vector2f(size.x, size.y);
    game_object[3].position = position2d + sf::Vector2f(0, size.y);

    game_object[0].texCoords = sf::Vector2f(0, 0);
    game_object[1].texCoords = sf::Vector2f(size.x, 0);
    game_object[2].texCoords = sf::Vector2f(size.x, size.y);
    game_object[3].texCoords = sf::Vector2f(0, size.y);
    
    if(colour != COLOUR_BLACK) {
        game_object[0].color = colour;
        game_object[1].color = colour;
        game_object[2].color = colour; 
        game_object[3].color = colour;
    }

    target.draw(game_object, states);
}