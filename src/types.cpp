#include "types.hpp"

using namespace core;

// Vector2f

Vector2f::Vector2f() 
    : x(0), y(0)
{}

Vector2f::Vector2f(float x, float y)
    : x(x), y(y)
{}

Vector2f::~Vector2f()
{}

void Vector2f::setX(float x) {
    this->x = x;
}

float Vector2f::getX() const {
    return this->x; 
}

void Vector2f::setY(float y) {
    this->y = y;
}

float Vector2f::getY() const {
    return this->y; 
}

sf::Vector2i Vector2f::asSFMLVector2i() const {
    return sf::Vector2i(this->x, this->y);
}

sf::Vector2f Vector2f::asSFMLVector2f() const {
    return sf::Vector2f(this->x, this->y);
}

// Vector2f Vector2f::operator+  (const Vector2f& vec) {
    
// }

// Vector2f& Vector2f::operator+= (const Vector2f& vec) {

// }

// Vector2f Vector2f::operator-  (const Vector2f& vec) {

// }

// Vector2f& Vector2f::operator-= (const Vector2f& vec) {

// }

// Vector2f Vector2f::operator*  (int multiplier) {

// }

// Vector2f& Vector2f::operator*= (int multiplier) {

// }  

// Vector2f Vector2f::operator/  (int divisor) {

// }

// Vector2f& Vector2f::operator/= (int divisor) {

// }

// Vector2i

Vector2i::Vector2i()
    : x(0), y(0)
{}

Vector2i::Vector2i(int x, int y)
    : x(x), y(y)
{}

Vector2i::~Vector2i()
{}

void Vector2i::setX(int x) {
    this->x = x;
}

int Vector2i::getX() const {
    return this->x; 
}

void Vector2i::setY(int y) {
    this->y = y;
}

int Vector2i::getY() const {
    return this->y; 
}

sf::Vector2i Vector2i::asSFMLVector2i() const {
    return sf::Vector2i(this->x, this->y);
}

sf::Vector2f Vector2i::asSFMLVector2f() const {
    return sf::Vector2f(this->x, this->y);
}

// Vector3f

Vector3f::Vector3f()
    : x(0), y(0), z(0)
{}

Vector3f::Vector3f(float x, float y, float z)
    : x(x), y(y), z(z)
{}

Vector3f::~Vector3f()
{}

void Vector3f::setX(float x) {
    this->x = x;
}

float Vector3f::getX() const {
    return this->x; 
}

void Vector3f::setY(float y) {
    this->y = y;
}

float Vector3f::getY() const {
    return this->y; 
}

void Vector3f::setZ(float z) {
    this->z = z;
}

float Vector3f::getZ() const {
    return this->z;
}

sf::Vector3i Vector3f::asSFMLVector3i() const {
    return sf::Vector3i(this->x, this->y, this->z);
}

sf::Vector3f Vector3f::asSFMLVector3f() const {
    return sf::Vector3f(this->x, this->y, this->z);
}

// Vector3i

Vector3i::Vector3i()
    : x(0), y(y), z(0)
{}

Vector3i::Vector3i(int x, int y, int z)
    : x(x), y(y), z(z)
{}

Vector3i::~Vector3i()
{}

void Vector3i::setX(int x) {
    this->x = x;
}

int Vector3i::getX() const {
    return this->x; 
}

void Vector3i::setY(int y) {
    this->y = y;
}

int Vector3i::getY() const {
    return this->y; 
}

void Vector3i::setZ(int z) {
    this->z = z;
}

int Vector3i::getZ() const {
    return this->z;
}

sf::Vector3i Vector3i::asSFMLVector3i() const {
    return sf::Vector3i(this->x, this->y, this->z);
}

sf::Vector3f Vector3i::asSFMLVector3f() const {
    return sf::Vector3f(this->x, this->y, this->z);
}

// Colour

Colour::Colour()
    : r(0), g(0), b(0)
{}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b)
    : r(r), g(g), b(b)
{}

Colour::~Colour()
{}

void Colour::setR(uint8_t r) {
    this->r = r;
}

uint8_t Colour::getR() const {
    return this->r; 
}

void Colour::setG(uint8_t g) {
    this->g = g;
}

uint8_t Colour::getG() const {
    return this->g; 
}

void Colour::setB(uint8_t b) {
    this->b = b;
}

uint8_t Colour::getB() const {
    return this->b;
}

sf::Color Colour::asSFMLColour() const {
    return sf::Color(this->r, this->g, this->b);
}