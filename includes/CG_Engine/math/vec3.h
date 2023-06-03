#ifndef CG_ENGINE_VEC3_H
#define CG_ENGINE_VEC3_H

#include <CG_Engine/math/common.h>

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

    Vec3 operator*(const Vec3 &vec) const {
        return {x * vec.x, y * vec.y, z * vec.z};
    }

    Vec3 operator/(float number) const {
        if (number != 0.f) {
            return {x / number, y / number, z / number};
        }
        throw std::logic_error("Divide by zero exception");
    };

    Vec3 &operator/=(float number) {
        if (number != 0.f) {
            x = x / number;
            y = y / number;
            z = z / number;
            return *this;
        }
        throw std::logic_error("Divide by zero exception");
    };

    float &operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Vec3 index out of range");
    }

    float operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Vec3 index out of range");
    }

    Vec3 abs() const {
        return {std::abs(x), std::abs(y), std::abs(z)};
    }

    float len() const {
        return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
    };

    Vec3 normalize() const {
        if (len() != 0.f) {
            return {x / len(), y / len(), z / len()};
        }
        return *this;
    };

    float dotProduct(const Vec3 &other) const {
        return (x * other.x) + (y * other.y) + (z * other.z);
    };

    Vec3 crossProduct(const Vec3 &other) const {
        return {y * other.z - other.y * z, z * other.x - other.z * x, x * other.y - other.x * y};
    };

    bool operator==(const Vec3 &other) const {
        return (x == other.x && y == other.y && z == other.z);
    };

    bool operator!=(const Vec3 &other) const {
        return !(x == other.x && y == other.y && z == other.z);
    };

    float x = 0.f, y = 0.f, z = 0.f;
};


#endif //CG_ENGINE_VEC3_H
