#include "types.hpp"
#include "globalutilities.hpp"
#include "simulationManager.hpp"

#include <iostream>

namespace core {
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

Vector2f& Vector2f::operator= (const Vector2f& vec) {
    this->x = vec.x;
    this->y = vec.y;
    return *this;
}

Vector2f Vector2f::operator+  (const Vector2f& vec) const {
    auto temp = Vector2f(this->x + vec.x, this->y + vec.y);
    return temp;
}

Vector2f& Vector2f::operator+= (const Vector2f& vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vector2f Vector2f::operator-  (const Vector2f& vec) const {
    auto temp = Vector2f(this->x - vec.x, this->y - vec.y);
    return temp;
}

Vector2f& Vector2f::operator-= (const Vector2f& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector2f Vector2f::operator*  (int multiplier) const {
    auto temp = Vector2f(this->x * multiplier, this->y * multiplier);
    return temp;
}

Vector2f& Vector2f::operator*= (int multiplier) {
    this->x *= multiplier;
    this->y *= multiplier;
    return *this;
}  

Vector2f Vector2f::operator/  (int divisor) const {
    auto temp = Vector2f(this->x / divisor, this->y / divisor);
    return temp;
}

Vector2f& Vector2f::operator/= (int divisor) {
    this->x /= divisor;
    this->y /= divisor;
    return *this;
}

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

Vector2i& Vector2i::operator= (const Vector2i& vec) {
    this->x = vec.x;
    this->y = vec.y;
    return *this;
}

Vector2i Vector2i::operator+  (const Vector2i& vec) const {
    auto temp = Vector2i(this->x + vec.x, this->y + vec.y);
    return temp;
}

Vector2i& Vector2i::operator+= (const Vector2i& vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vector2i Vector2i::operator-  (const Vector2i& vec) const {
    auto temp = Vector2i(this->x - vec.x, this->y - vec.y);
    return temp;
}

Vector2i& Vector2i::operator-= (const Vector2i& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector2i Vector2i::operator*  (int multiplier) const {
    auto temp = Vector2i(this->x * multiplier, this->y * multiplier);
    return temp;
}

Vector2i& Vector2i::operator*= (int multiplier) {
    this->x *= multiplier;
    this->y *= multiplier;
    return *this;
}  

Vector2i Vector2i::operator/  (int divisor) const {
    auto temp = Vector2i(this->x / divisor, this->y / divisor);
    return temp;
}

Vector2i& Vector2i::operator/= (int divisor) {
    this->x /= divisor;
    this->y /= divisor;
    return *this;
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

Vector3f& Vector3f::operator= (const Vector3f& vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    return *this;
}

Vector3f Vector3f::operator+  (const Vector3f& vec) const {
    auto temp = Vector3f(this->x + vec.x, this->y + vec.y, this->z + vec.z);
    return temp;
}

Vector3f& Vector3f::operator+= (const Vector3f& vec) {
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    return *this;
}

Vector3f Vector3f::operator-  (const Vector3f& vec) const {
    auto temp = Vector3f(this->x - vec.x, this->y - vec.y, this->z - vec.z);
    return temp;
}

Vector3f& Vector3f::operator-= (const Vector3f& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    return *this;
}

Vector3f Vector3f::operator*  (int multiplier) const {
    auto temp = Vector3f(this->x * multiplier, this->y * multiplier, this->z * multiplier);
    return temp;
}

Vector3f& Vector3f::operator*= (int multiplier) {
    this->x *= multiplier;
    this->y *= multiplier;
    this->z *= multiplier;
    return *this;
}  

Vector3f Vector3f::operator/  (int divisor) const {
    auto temp = Vector3f(this->x / divisor, this->y / divisor, this->y / divisor);
    return temp;
}

Vector3f& Vector3f::operator/= (int divisor) {
    this->x /= divisor;
    this->y /= divisor;
    this->z /= divisor;
    return *this;
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

Vector3i& Vector3i::operator= (const Vector3i& vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    return *this;
}

Vector3i Vector3i::operator+  (const Vector3i& vec) const {
    auto temp = Vector3i(this->x + vec.x, this->y + vec.y, this->z + vec.z);
    return temp;
}

Vector3i& Vector3i::operator+= (const Vector3i& vec) {
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    return *this;
}

Vector3i Vector3i::operator-  (const Vector3i& vec) const {
    auto temp = Vector3i(this->x - vec.x, this->y - vec.y, this->z - vec.z);
    return temp;
}

Vector3i& Vector3i::operator-= (const Vector3i& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    return *this;
}

Vector3i Vector3i::operator*  (int multiplier) const {
    auto temp = Vector3i(this->x * multiplier, this->y * multiplier, this->z * multiplier);
    return temp;
}

Vector3i& Vector3i::operator*= (int multiplier) {
    this->x *= multiplier;
    this->y *= multiplier;
    this->z *= multiplier;
    return *this;
}  

Vector3i Vector3i::operator/  (int divisor) const {
    auto temp = Vector3i(this->x / divisor, this->y / divisor, this->y / divisor);
    return temp;
}

Vector3i& Vector3i::operator/= (int divisor) {
    this->x /= divisor;
    this->y /= divisor;
    this->z /= divisor;
    return *this;
}

// Colour

Colour::Colour()
    : r(0), g(0), b(0), a(255)
{}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b)
    : r(r), g(g), b(b), a(255)
{}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r(r), g(g), b(b), a(a)
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

void Colour::setA(uint8_t a) {
    this->a = a;
}

uint8_t Colour::getA() const {
    return this->a;
}

sf::Color Colour::asSFMLColour() const {
    return sf::Color(this->r, this->g, this->b, this->a);
}

Colour& Colour::operator= (const Colour& colour) {
    this->r = colour.r;
    this->g = colour.g;
    this->b = colour.b;
    this->a = colour.a;
    return *this;
}

bool Colour::operator== (const Colour& colour) const {
    return (
        this->r == colour.r &&
        this->g == colour.g &&
        this->b == colour.b &&
        this->a == colour.a
    );
}

bool Colour::operator== (const sf::Color& colour) const {
    return (
        this->r == colour.r &&
        this->g == colour.g &&
        this->b == colour.b &&
        this->a == colour.a
    );
}

// Ratio

Ratio::Ratio()
    : expression_x("0%"), expression_y("y"), x(0), y(0)
{}

Ratio::Ratio(const std::string& x, const std::string& y)
    : expression_x(x), expression_y(y)
{
    this->x = this->extractRatio(x, true);
    this->y = this->extractRatio(y, false);
}

Ratio::~Ratio()
{}

void Ratio::setX(const std::string& x) {
    this->expression_x = x;
}

const std::string& Ratio::getX() const {
    return this->expression_x;
}

void Ratio::setY(const std::string& y) {
    this->expression_y = y;
}

const std::string& Ratio::getY() const {
    return this->expression_y;
}

core::Vector2f Ratio::asNumber() const {
    return core::Vector2f(this->x, this->y);
} 

float Ratio::extractRatio(const std::string& coordinate, bool is_x_coordinate) {
    if(!iso::containsWord(coordinate, "%"))
        iso::printError("Ratio::extractRatio()", "Invalid ratio description");
    
    auto value = iso::readBefore(coordinate, "%"); 
    float ratio = std::stof(value) / 100.0f;        
    return ratio;    
}
}