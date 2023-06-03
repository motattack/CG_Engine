#ifndef CG_ENGINE_MAT2X2_H
#define CG_ENGINE_MAT2X2_H

#include <CG_Engine/math/common.h>

class Mat2x2 {
public:
    Mat2x2() = default;

    explicit Mat2x2(float number) : matrix{{number, 0.f},
                                           {0.f,      number}} {}

    Mat2x2(float a, float b, float c, float d) :
            matrix{{a, c},
                   {b, d}} {};

    Mat2x2(const Vec2 &first, const Vec2 &second) :
            matrix{{first.x, second.x},
                   {first.y, second.y}} {};

    Mat2x2(const Mat2x2 &other) = default;

    Mat2x2 operator+(const Mat2x2 &other) const {
        return {matrix[0][0] + other.matrix[0][0],
                matrix[0][1] + other.matrix[0][1],
                matrix[1][0] + other.matrix[1][0],
                matrix[1][1] + other.matrix[1][1]};
    };

    Mat2x2 &operator+=(const Mat2x2 &other) {
        *this = *this + other;
        return *this;
    };

    Mat2x2 operator-(const Mat2x2 &other) const {
        return {matrix[0][0] - other.matrix[0][0],
                matrix[0][1] - other.matrix[0][1],
                matrix[1][0] - other.matrix[1][0],
                matrix[1][1] - other.matrix[1][1]};
    };

    Mat2x2 &operator-=(const Mat2x2 &other) {
        *this = *this - other;
        return *this;
    };

    Mat2x2 operator*(const Mat2x2 &other) const {
        return {matrix[0][0] * other.matrix[0][0] + matrix[0][1] * other.matrix[1][0],
                matrix[0][0] * other.matrix[0][1] + matrix[0][1] * other.matrix[1][1],
                matrix[1][0] * other.matrix[0][0] + matrix[1][1] * other.matrix[1][0],
                matrix[1][0] * other.matrix[0][1] + matrix[1][1] * other.matrix[1][1]};
    };

    Mat2x2 &operator*=(const Mat2x2 &other) {
        *this = *this * other;
        return *this;
    };

    Mat2x2 operator*(float number) const {
        return {matrix[0][0] * number,
                matrix[0][1] * number,
                matrix[1][0] * number,
                matrix[1][1] * number};
    };

    Mat2x2 &operator*=(float number) {
        *this = *this * number;
        return *this;
    };

    Mat2x2 operator/(float number) const {
        if (number != 0.f) {
            return {matrix[0][0] / number,
                    matrix[0][1] / number,
                    matrix[1][0] / number,
                    matrix[1][1] / number};
        }
        throw std::logic_error("Divide by zero exception");
    };

    Mat2x2 &operator/=(float number) {
        if (number != 0.f) {
            *this = *this / number;
            return *this;
        }
        throw std::logic_error("Divide by zero exception");
    };

    Mat2x2 operator+(float number) const {
        return {matrix[0][0] + number,
                matrix[0][1] + number,
                matrix[1][0] + number,
                matrix[1][1] + number};
    };

    Mat2x2 &operator+=(float number) {
        *this = *this + number;
        return *this;
    };

    Mat2x2 operator-(float number) const {
        return {matrix[0][0] - number,
                matrix[0][1] - number,
                matrix[1][0] - number,
                matrix[1][1] - number};
    };

    Mat2x2 &operator-=(float number) {
        *this = *this - number;
        return *this;
    };

    Vec2 operator*(const Vec2 &other) const {
        return {matrix[0][0] * other.x + matrix[0][1] * other.y,
                matrix[1][0] * other.x + matrix[1][1] * other.y};
    };

    static Mat2x2 identity() {
        return Mat2x2(1.f);
    };

    float det() {
        return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
    }

    Mat2x2 reverse() {
        float divideDet = 1.f / det();

        return {matrix[1][1] * divideDet,
                -matrix[0][1] * divideDet,
                -matrix[1][0] * divideDet,
                +matrix[0][0] * divideDet};
    };

    Mat2x2 transpose() {
        return Mat2x2(Column(0), Column(1));
    };

    Vec2 Row(int index) {
        if (index >= 0 && index < 2) {
            return {matrix[index][0], matrix[index][1]};
        }
        return Vec2(0.f);
    }

    Vec2 Column(int index) {
        if (index >= 0 && index < 2) {
            float res[2];
            for (int i = 0; i < 2; i++) {
                res[i] = matrix[i][index];
            }
            return {res[0], res[1]};
        }
        return Vec2(0.f);
    };

    float *operator[](int row) {
        return matrix[row];
    }

    bool operator==(const Mat2x2 &other) {
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                if (matrix[i][j] != other.matrix[i][j])
                    return false;
        return true;
    };

    bool operator!=(const Mat2x2 &other) {
        return matrix != other.matrix;
    };

    float matrix[2][2]{};
};

#endif //CG_ENGINE_MAT2X2_H
