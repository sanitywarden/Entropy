#pragma once

#include <stdint.h>
#include <string>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>

namespace core {
class Vector2f {
    public:
        float x;
        float y;

    public:
        Vector2f();
        Vector2f(float x, float y);
        ~Vector2f();

        void setX(float x);
        float getX() const;

        void setY(float y);
        float getY() const;

        sf::Vector2i asSFMLVector2i() const;
        sf::Vector2f asSFMLVector2f() const;

        Vector2f& operator=  (const Vector2f& vec);
        Vector2f  operator+  (const Vector2f& vec) const;
        Vector2f& operator+= (const Vector2f& vec);
        Vector2f  operator-  (const Vector2f& vec) const;
        Vector2f& operator-= (const Vector2f& vec);
        Vector2f  operator*  (int multiplier) const;
        Vector2f& operator*= (int multiplier);  
        Vector2f  operator/  (int divisor) const;
        Vector2f& operator/= (int divisor);
};

class Vector2i {
    public:
        int x;
        int y;
    
    public:
        Vector2i();
        Vector2i(int x, int y);
        ~Vector2i();

        void setX(int x);
        int getX() const;

        void setY(int y);
        int getY() const;

        sf::Vector2i asSFMLVector2i() const;
        sf::Vector2f asSFMLVector2f() const;

        Vector2i& operator=  (const Vector2i& vec);
        Vector2i  operator+  (const Vector2i& vec) const;
        Vector2i& operator+= (const Vector2i& vec);
        Vector2i  operator-  (const Vector2i& vec) const;
        Vector2i& operator-= (const Vector2i& vec);
        Vector2i  operator*  (int multiplier) const;
        Vector2i& operator*= (int multiplier);  
        Vector2i  operator/  (int divisor) const;
        Vector2i& operator/= (int divisor);
};

class Vector3f {
    public:
        float x;
        float y;
        float z;

    public:
        Vector3f();
        Vector3f(float x, float y, float z);
        ~Vector3f();

        void setX(float x);
        float getX() const;

        void setY(float y);
        float getY() const;

        void setZ(float z);
        float getZ() const;

        sf::Vector3i asSFMLVector3i() const;
        sf::Vector3f asSFMLVector3f() const;

        Vector3f& operator=  (const Vector3f& vec);
        Vector3f  operator+  (const Vector3f& vec) const;
        Vector3f& operator+= (const Vector3f& vec);
        Vector3f  operator-  (const Vector3f& vec) const;
        Vector3f& operator-= (const Vector3f& vec);
        Vector3f  operator*  (int multiplier) const;
        Vector3f& operator*= (int multiplier);  
        Vector3f  operator/  (int divisor) const;
        Vector3f& operator/= (int divisor);
};

class Vector3i {
    public:
        int x;
        int y;
        int z;
    
    public:
        Vector3i();
        Vector3i(int x, int y, int z);
        ~Vector3i();

        void setX(int x);
        int getX() const;

        void setY(int y);
        int getY() const;

        void setZ(int z);
        int getZ() const;

        sf::Vector3i asSFMLVector3i() const;
        sf::Vector3f asSFMLVector3f() const;

        Vector3i& operator=  (const Vector3i& vec);
        Vector3i  operator+  (const Vector3i& vec) const;
        Vector3i& operator+= (const Vector3i& vec);
        Vector3i  operator-  (const Vector3i& vec) const;
        Vector3i& operator-= (const Vector3i& vec);
        Vector3i  operator*  (int multiplier) const;
        Vector3i& operator*= (int multiplier);  
        Vector3i  operator/  (int divisor) const;
        Vector3i& operator/= (int divisor);
};

class Colour {
    public:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

    public: 
        Colour();
        Colour(uint8_t r, uint8_t g, uint8_t b);
        Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        ~Colour();

        void setR(uint8_t r);
        uint8_t getR() const;

        void setG(uint8_t g);
        uint8_t getG() const;

        void setB(uint8_t b);
        uint8_t getB() const;

        void setA(uint8_t a);
        uint8_t getA() const;

        sf::Color asSFMLColour() const;

        Colour& operator=  (const Colour& colour);
        bool    operator== (const Colour& colour) const;         
        bool    operator== (const sf::Color& colour) const;
};

class Ratio {
    private:
        std::string expression_x;
        std::string expression_y;
        float x;
        float y;
    private:
        float extractRatio(const std::string& coordinate, bool is_x_coordinate);
    public:
        Ratio();
        Ratio(const std::string& x, const std::string& y);
        ~Ratio();

        void setX(const std::string& x);
        const std::string& getX() const;

        void setY(const std::string& y);
        const std::string& getY() const;

        core::Vector2f asNumber() const;
};
}