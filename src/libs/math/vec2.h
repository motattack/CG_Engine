#ifndef CG_ENGINE_VEC2_H
#define CG_ENGINE_VEC2_H

#include <cmath>

class Vec2 {
public:
    Vec2() = default;

    explicit Vec2(float number = 0.0f) : x(number), y(number) {};

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
        if (number != 0) {
            return {x / number, y / number};
        }
        return *this;
    };

    Vec2 &operator/=(float number) {
        if (number != 0) {
            x = x / number;
            y = y / number;
            return *this;
        }
        return *this;
    };

    [[nodiscard]] float len() const {
        return sqrtf(powf(x, 2) + powf(y, 2));
    };

    [[nodiscard]] Vec2 normalize() const {
        if (len() != 0) {
            return {x / len(), y / len()};
        }
        return *this;
    };

    [[nodiscard]] float dotProduct(const Vec2 &other) const {
        return (x * other.x) + (y * other.y);
    };

    bool operator==(const Vec2 &other) const {
        return (x == other.x && y == other.y);
    };

    bool operator!=(const Vec2 &other) const {
        return !(x == other.x && y == other.y);
    };

    float x = 0, y = 0;
};

#endif //CG_ENGINE_VEC2_H
