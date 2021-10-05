#include "entity.hpp"

using namespace iso;

Entity::Entity() {
    this->parent_position   = sf::Vector2f(0, 0);
    this->relative_position = sf::Vector2f(0, 0);
    this->size              = sf::Vector2f(0, 0);
    this->texture           = nullptr;
}

Entity::Entity(sf::Vector2f parent_position, sf::Vector2f relative_position, sf::Vector2f size, sf::Texture* texture) {
    this->parent_position   = parent_position;
    this->relative_position = relative_position;
    this->size              = size;
    this->texture           = texture; 
}

Entity::~Entity() {

}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray entity(sf::Quads, 4);

    sf::Vector2f transformed_position = this->parent_position + this->relative_position;

    entity[0].position = transformed_position;
    entity[1].position = transformed_position + sf::Vector2f(this->size.x, 0);
    entity[2].position = transformed_position + sf::Vector2f(this->size.x, this->size.y);
    entity[3].position = transformed_position + sf::Vector2f(0, this->size.y);

    entity[0].texCoords = sf::Vector2f(0, 0);
    entity[1].texCoords = sf::Vector2f(this->size.x, 0);
    entity[2].texCoords = sf::Vector2f(this->size.x, this->size.y);
    entity[3].texCoords = sf::Vector2f(0, this->size.y);

    states.texture = this->texture;
    target.draw(entity, states);
}

// If the entity texture pointer has a value, then the entity exists.
// If it is a nullptr, then it does not.
bool Entity::exists() const {
    return (this->texture != nullptr) ? true : false;
}