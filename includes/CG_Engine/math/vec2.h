#ifndef CG_ENGINE_VEC2_H
#define CG_ENGINE_VEC2_H

#include <CG_Engine/math/common.h>

class Vec2 {
public:
    explicit Vec2(float number = 0.f) : x(number), y(number) {};

    Vec2(float x, float y) : x(x), y(y) {};

    Vec2(const Vec2 &other) = default;

    Vec2 operator+(const Vec2 &other) const {
        return {x + other.x, y + other.y};
    };

    Vec2 &operator+=(const Vec2 &other) {
        *this = *this + other;
        return *this;
    };

    Vec2 operator-(const Vec2 &other) const {
        return {x - other.x, y - other.y};
    };

    Vec2 &operator-=(const Vec2 &other) {
        *this = *this - other;
        return *this;
    };

    Vec2 operator*(float number) const {
        return {x * number, y * number};
    };

    Vec2 &operator*=(float number) {
        x = x * number;
        y = y * number;
        return *this;
    };

    Vec2 operator/(float number) const {
        if (number != 0.f) {
            return {x / number, y / number};
        }
        throw std::logic_error("Divide by zero exception");
    };

    Vec2 &operator/=(float number) {
        if (number != 0.f) {
            x = x / number;
            y = y / number;
            return *this;
        }
        throw std::logic_error("Divide by zero exception");
    };

    float len() const {
        return sqrtf(powf(x, 2) + powf(y, 2));
    };

    Vec2 normalize() const {
        if (len() != 0.f) {
            return {x / len(), y / len()};
        }
        return *this;
    };

    float dotProduct(const Vec2 &other) const {
        return (x * other.x) + (y * other.y);
    };

    bool operator==(const Vec2 &other) const {
        return (x == other.x && y == other.y);
    };

    bool operator!=(const Vec2 &other) const {
        return !(x == other.x && y == other.y);
    };

    float x = 0.f, y = 0.f;
};

#endif //CG_ENGINE_VEC2_H
