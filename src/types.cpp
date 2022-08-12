#include "types.hpp"

// Vector2f

core::Vector2f::Vector2f() 
    : x(0), y(0)
{}

core::Vector2f::Vector2f(float x, float y)
    : x(x), y(y)
{}

core::Vector2f::~Vector2f()
{}

void core::Vector2f::setX(float x) {
    this->x = x;
}

float core::Vector2f::getX() const {
    return this->x; 
}

void core::Vector2f::setY(float y) {
    this->y = y;
}

float core::Vector2f::getY() const {
    return this->y; 
}

sf::Vector2i core::Vector2f::asSFMLVector2i() const {
    return sf::Vector2i(this->x, this->y);
}

sf::Vector2f core::Vector2f::asSFMLVector2f() const {
    return sf::Vector2f(this->x, this->y);
}


// Vector2i

core::Vector2i::Vector2i()
    : x(0), y(0)
{}

core::Vector2i::Vector2i(int x, int y)
    : x(x), y(y)
{}

core::Vector2i::~Vector2i()
{}

void core::Vector2i::setX(int x) {
    this->x = x;
}

int core::Vector2i::getX() const {
    return this->x; 
}

void core::Vector2i::setY(int y) {
    this->y = y;
}

int core::Vector2i::getY() const {
    return this->y; 
}

sf::Vector2i core::Vector2i::asSFMLVector2i() const {
    return sf::Vector2i(this->x, this->y);
}

sf::Vector2f core::Vector2i::asSFMLVector2f() const {
    return sf::Vector2f(this->x, this->y);
}

// Vector3f

core::Vector3f::Vector3f()
    : x(0), y(0), z(0)
{}

core::Vector3f::Vector3f(float x, float y, float z)
    : x(x), y(y), z(z)
{}

core::Vector3f::~Vector3f()
{}

void core::Vector3f::setX(float x) {
    this->x = x;
}

float core::Vector3f::getX() const {
    return this->x; 
}

void core::Vector3f::setY(float y) {
    this->y = y;
}

float core::Vector3f::getY() const {
    return this->y; 
}

void core::Vector3f::setZ(float z) {
    this->z = z;
}

float core::Vector3f::getZ() const {
    return this->z;
}

sf::Vector3i core::Vector3f::asSFMLVector3i() const {
    return sf::Vector3i(this->x, this->y, this->z);
}

sf::Vector3f core::Vector3f::asSFMLVector3f() const {
    return sf::Vector3f(this->x, this->y, this->z);
}

// Vector3i

core::Vector3i::Vector3i()
    : x(0), y(y), z(0)
{}

core::Vector3i::Vector3i(int x, int y, int z)
    : x(x), y(y), z(z)
{}

core::Vector3i::~Vector3i()
{}

void core::Vector3i::setX(int x) {
    this->x = x;
}

int core::Vector3i::getX() const {
    return this->x; 
}

void core::Vector3i::setY(int y) {
    this->y = y;
}

int core::Vector3i::getY() const {
    return this->y; 
}

void core::Vector3i::setZ(int z) {
    this->z = z;
}

int core::Vector3i::getZ() const {
    return this->z;
}

sf::Vector3i core::Vector3i::asSFMLVector3i() const {
    return sf::Vector3i(this->x, this->y, this->z);
}

sf::Vector3f core::Vector3i::asSFMLVector3f() const {
    return sf::Vector3f(this->x, this->y, this->z);
}

// Colour

core::Colour::Colour()
    : r(0), g(0), b(0)
{}

core::Colour::Colour(uint8_t r, uint8_t g, uint8_t b)
    : r(r), g(g), b(b)
{}

core::Colour::~Colour()
{}

void core::Colour::setR(uint8_t r) {
    this->r = r;
}

uint8_t core::Colour::getR() const {
    return this->r; 
}

void core::Colour::setG(uint8_t g) {
    this->g = g;
}

uint8_t core::Colour::getG() const {
    return this->g; 
}

void core::Colour::setB(uint8_t b) {
    this->b = b;
}

uint8_t core::Colour::getB() const {
    return this->b;
}

sf::Color core::Colour::asSFMLColour() const {
    return sf::Color(this->r, this->g, this->b);
}