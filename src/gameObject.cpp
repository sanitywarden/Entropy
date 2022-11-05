#include "gameObject.hpp"
#include "colours.hpp"
#include "globalutilities.hpp"
#include "simulationManager.hpp"

#include <iostream>

namespace iso {
int GameObject::instance_id = 0;
GameObject::GameObject() 
    : object_name("*"), object_position(core::Vector3i(0, 0, 0)), object_size(core::Vector2i(0, 0)), object_texture_name("*"), object_colour(COLOUR_BLACK), index(-1)
{ instance_id++; }

GameObject::GameObject(const GameObject& object) 
    : object_name(object.getName()), object_position(object.getPosition()), object_size(object.getSize()), object_texture_name(object.getTextureName()), object_colour(object.getColour()), index(object.getIndex())
{ instance_id++; }

GameObject::GameObject(core::Vector3i position, core::Vector3i relative_position, core::Vector2i size, const std::string& texture_name) 
    : object_name("*"), object_position(position + relative_position), object_size(size), object_texture_name(texture_name), object_colour(COLOUR_BLACK), index(-1)
{ instance_id++; }

GameObject::GameObject(core::Vector3i position, core::Vector3i relative_position, core::Vector2i size, const std::string& texture_name, const std::string& object_name) 
    : object_name(object_name), object_position(position + relative_position), object_size(size), object_texture_name(texture_name), object_colour(COLOUR_BLACK), index(-1)
{ instance_id++; }

GameObject::GameObject(core::Vector3i position, core::Vector2i size, const std::string& texture_name)
    : object_name("*"), object_position(position), object_size(size), object_texture_name(texture_name), object_colour(COLOUR_BLACK), index(-1)
{ instance_id++; }

GameObject::~GameObject() 
{}

bool GameObject::exists() const {
    return this->object_texture_name[0] != '*';
}

bool GameObject::hasName() const {
    return this->object_name[0] != '*';
}

bool GameObject::contains(core::Vector2i point) const {
    auto position2d = this->getPosition2D();    
    sf::IntRect object_rectangle(position2d.asSFMLVector2i(), this->getSize().asSFMLVector2i());
    return object_rectangle.contains(point.asSFMLVector2i());
}

const std::string& GameObject::getName() const {
    return this->object_name;
}

core::Vector3i GameObject::getPosition() const {
    return this->object_position;
}

// When you represent a 3D object as a 2D isometric object, you just subtract the Z axis from the Y axis.
core::Vector2i GameObject::getPosition2D() const {
    return core::Vector2i(this->object_position.x, this->object_position.y + this->object_position.z);
}

core::Vector2i GameObject::getSize() const {
    return this->object_size;
}

const std::string& GameObject::getTextureName() const {
    return this->object_texture_name;
}

core::Colour GameObject::getColour() const {
    return this->object_colour;
}

int GameObject::getInstanceId() const {
    return instance_id;
}

int GameObject::getIndex() const {
    return this->index;
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray game_object(sf::Quads, 4);

    auto position2d = this->getPosition2D().asSFMLVector2f();
    auto size       = this->getSize().asSFMLVector2f();
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
        game_object[0].color = colour.asSFMLColour();
        game_object[1].color = colour.asSFMLColour();
        game_object[2].color = colour.asSFMLColour();
        game_object[3].color = colour.asSFMLColour();
    }

    target.draw(game_object, states);
}

void GameObject::setName(const std::string& name) {
    this->object_name = name; 
}

void GameObject::setPosition(core::Vector3i position) {
    this->object_position = position; 
}

void GameObject::setSize(core::Vector2i size) {
    this->object_size =  size;
}

void GameObject::setTextureName(const std::string& texture_name) {
    this->object_texture_name = texture_name; 
}

void GameObject::setColour(core::Colour colour) {
    this->object_colour = colour;
}
}