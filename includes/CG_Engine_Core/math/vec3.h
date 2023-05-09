#ifndef CG_ENGINE_VEC3_H
#define CG_ENGINE_VEC3_H

#include <cmath>

class Vec3 {
public:
    Vec3() = default;

    explicit Vec3(float number) : x(number), y(number), z(number) {};

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {};

    Vec3(const Vec3 &other) = default;

    Vec3 operator+(const Vec3 &other) const {
        return {x + other.x, y + other.y, z + other.z};
    };

    Vec3 &operator+=(const Vec3 &other) {
        *this = *this + other;
        return *this;
    };

    Vec3 &operator+=(const float other) {
        *this = *this + Vec3(other);
        return *this;
    };

    Vec3 operator-(const Vec3 &other) const {
        return {x - other.x, y - other.y, z - other.z};
    };

    Vec3 operator-() const {
        return {-x, -y, -z};
    };

    Vec3 &operator-=(const Vec3 &other) {
        *this = *this - other;
        return *this;
    };

    Vec3 operator*(float number) const {
        return {x * number, y * number, z * number};
    };

    Vec3 &operator*=(float number) {
        x = x * number;
        y = y * number;
        z = z * number;
        return *this;
    };

    Vec3 operator/(float number) const {
        if (number != 0) {
            return {x / number, y / number, z / number};
        }
        return *this;
    };

    Vec3 &operator/=(float number) {
        if (number != 0) {
            x = x / number;
            y = y / number;
            z = z / number;
            return *this;
        }
        return *this;
    };

    [[nodiscard]] float len() const {
        return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
    };

    [[nodiscard]] Vec3 normalize() const {
        if (len() != 0) {
            return {x / len(), y / len(), z / len()};
        }
        return *this;
    };

    [[nodiscard]] float dotProduct(const Vec3 &other) const {
        return (x * other.x) + (y * other.y) + (z * other.z);
    };

    [[nodiscard]] Vec3 crossProduct(const Vec3 &other) const {
        return {y * other.z - other.y * z, z * other.x - other.z * x, x * other.y - other.x * y};
    };

    bool operator==(const Vec3 &other) const {
        return (x == other.x && y == other.y && z == other.z);
    };

    bool operator!=(const Vec3 &other) const {
        return !(x == other.x && y == other.y && z == other.z);
    };

    float x = 0, y = 0, z = 0;
};


#endif //CG_ENGINE_VEC3_H
