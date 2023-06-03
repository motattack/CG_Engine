#ifndef CG_ENGINE_VEC4_H
#define CG_ENGINE_VEC4_H

#include <CG_Engine/math/common.h>

class Vec4 {
public:
    Vec4() = default;

    explicit Vec4(float number) : x(number), y(number), z(number), w(number) {};

    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

    Vec4(const Vec4 &other) = default;

    Vec4 operator+(const Vec4 &other) const {
        return {x + other.x, y + other.y, z + other.z, w + other.w};
    };

    Vec4 &operator+=(const Vec4 &other) {
        *this = *this + other;
        return *this;
    };

    Vec4 operator-(const Vec4 &other) const {
        return {x - other.x, y - other.y, z - other.z, w - other.w};
    };

    Vec4 &operator-=(const Vec4 &other) {
        *this = *this - other;
        return *this;
    };

    Vec4 operator*(float number) const {
        return {x * number, y * number, z * number, w * number};
    };

    Vec4 &operator*=(float number) {
        x = x * number;
        y = y * number;
        z = z * number;
        w = w * number;
        return *this;
    };

    Vec4 operator/(float number) const {
        if (number != 0) {
            return {x / number, y / number, z / number, w / number};
        }
        throw std::logic_error("Divide by zero exception");
    };

    Vec4 &operator/=(float number) {
        if (number != 0) {
            x = x / number;
            y = y / number;
            z = z / number;
            w = w / number;
            return *this;
        }
        throw std::logic_error("Divide by zero exception");
    };

    float len() const {
        return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2) + powf(w, 2));
    };

    Vec4 normalize() const {
        if (len() != 0) {
            return {x / len(), y / len(), z / len(), w / len()};
        }
        return *this;
    };

    float dotProduct(const Vec4 &other) const {
        return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
    };

    bool operator==(const Vec4 &other) const {
        return (x == other.x && y == other.y && z == other.z && w == other.w);
    };

    bool operator!=(const Vec4 &other) const {
        return !(x == other.x && y == other.y && z == other.z && w == other.w);
    };

    float x = 0.f, y = 0.f, z = 0.f, w = 0.f;
};

#endif //CG_ENGINE_VEC4_H
