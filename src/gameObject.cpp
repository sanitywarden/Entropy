#include "gameObject.hpp"
#include "colours.hpp"

#include <iostream>

using namespace iso;

GameObject::GameObject() {
    this->object_name         = "*";
    this->object_position     = sf::Vector2f(0, 0);
    this->object_size         = sf::Vector2f(0, 0);
    this->object_texture_name = "*";
    this->object_colour       = COLOUR_BLACK;
}

GameObject::GameObject(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name) {
    this->object_name         = "*";
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
    sf::Rect object_rectangle(this->object_position, this->object_size);
    return object_rectangle.contains(point);
}

std::string GameObject::getName() const {
    return this->object_name;
}

sf::Vector2f GameObject::getPosition() const {
    return this->object_position;
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

    game_object[0].position = this->object_position;
    game_object[1].position = this->object_position + sf::Vector2f(this->object_size.x, 0);
    game_object[2].position = this->object_position + sf::Vector2f(this->object_size.x, this->object_size.y);
    game_object[3].position = this->object_position + sf::Vector2f(0, this->object_size.y);

    game_object[0].texCoords = sf::Vector2f(0, 0);
    game_object[1].texCoords = sf::Vector2f(this->object_size.x, 0);
    game_object[2].texCoords = sf::Vector2f(this->object_size.x, this->object_size.y);
    game_object[3].texCoords = sf::Vector2f(0, this->object_size.y);
    
    auto colour = this->object_colour;
    if(colour != COLOUR_BLACK) {
        game_object[0].color = colour;
        game_object[1].color = colour;
        game_object[2].color = colour; 
        game_object[3].color = colour;
    }

    target.draw(game_object, states);
}