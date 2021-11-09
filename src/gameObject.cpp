#include "gameObject.hpp"

using namespace iso;

GameObject::GameObject() {
    this->object_position = sf::Vector2f(0, 0);
    this->object_size     = sf::Vector2f(0, 0);
    this->object_texture  = nullptr;
}

GameObject::GameObject(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, sf::Texture* texture) {
    this->object_position = position + relative_position;
    this->object_size     = size;
    this->object_texture  = texture;
}

GameObject::~GameObject() {

}

bool GameObject::exists() const {
    return this->object_texture == nullptr ? false : true;
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

    states.texture = this->object_texture;
    target.draw(game_object, states);
}