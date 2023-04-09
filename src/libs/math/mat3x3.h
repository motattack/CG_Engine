#ifndef CG_ENGINE_MAT3X3_H
#define CG_ENGINE_MAT3X3_H

#include <cmath>
#include "vec3.h"
#include <vector>
#include <array>

class Mat3x3 {
public:
    Mat3x3() = default;

    explicit Mat3x3(float number) {
        matrix[0][0] = number; matrix[0][1] = 0; matrix[0][2] = 0;
        matrix[1][0] = 0; matrix[1][1] = number; matrix[1][2] = 0;
        matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = number;
    };


    Mat3x3(float a, float b, float c,
           float d, float e, float f,
           float g, float h, float i) {
        matrix[0][0] = a; matrix[0][1] = d; matrix[0][2] = g;
        matrix[1][0] = b; matrix[1][1] = e; matrix[1][2] = h;
        matrix[2][0] = c; matrix[2][1] = f; matrix[2][2] = i;
    }

    Mat3x3(const Vec3 &first, const Vec3 &second, const Vec3 &third) {
        matrix[0][0] = first.x; matrix[0][1] = second.x; matrix[0][2] = third.x;
        matrix[1][0] = first.y; matrix[1][1] = second.y; matrix[1][2] = third.y;
        matrix[2][0] = first.z; matrix[2][1] = second.z; matrix[2][2] = third.z;
    }

    Mat3x3(const Mat3x3 &other) = default;

    Mat3x3 operator+(const Mat3x3 &other) const {
        Mat3x3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.matrix[i][j] = matrix[j][i] + other.matrix[j][i];
        return result;
    };

    Mat3x3 &operator+=(const Mat3x3 &other) {
        *this = *this + other;
        return *this;
    };

    Mat3x3 operator-(const Mat3x3 &other) const {
        Mat3x3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.matrix[i][j] = matrix[j][i] - other.matrix[j][i];
        return result;
    }

    Mat3x3 &operator-=(const Mat3x3 &other) {
        *this = *this - other;
        return *this;
    }

    Mat3x3 operator*(const Mat3x3 &other) const {
        Mat3x3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                result.matrix[j][i] = 0;
                for (int k = 0; k < 3; k++)
                    result.matrix[j][i] += matrix[i][k] * other.matrix[k][j];

            }
        return result;
    }

    Mat3x3 &operator*=(const Mat3x3 &other) {
        *this = *this * other;
        return *this;
    }

    Mat3x3 operator*(float number) const {
        Mat3x3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.matrix[i][j] = matrix[j][i] * number;
        return result;
    }

    Mat3x3 &operator*=(float number) {
        *this = *this * number;
        return *this;
    }

    Mat3x3 operator/(float number) const {
        Mat3x3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.matrix[i][j] = matrix[j][i] / number;
        return result;
    }

    Mat3x3 &operator/=(float number) {
        *this = *this / number;
        return *this;
    }

    Mat3x3 operator+(float number) const {
        Mat3x3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.matrix[i][j] = matrix[j][i] + number;
        return result;
    }

    Mat3x3 &operator+=(float number) {
        *this = *this + number;
        return *this;
    }

    Mat3x3 operator-(float number) const {
        Mat3x3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.matrix[i][j] = matrix[j][i] - number;
        return result;
    }

    Mat3x3 &operator-=(float number) {
        *this = *this - number;
        return *this;
    }

    Vec3 operator*(const Vec3 &vec) const {
        Vec3 result;
        result.x = matrix[0][0] * vec.x + matrix[0][1] * vec.y + matrix[0][2] * vec.z;
        result.y = matrix[1][0] * vec.x + matrix[1][1] * vec.y + matrix[1][2] * vec.z;
        result.z = matrix[2][0] * vec.x + matrix[2][1] * vec.y + matrix[2][2] * vec.z;
        return result;
    }

    static Mat3x3 identity() {
        return Mat3x3(1.0f);
    };

    Mat3x3 reverse() {
        float det = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[2][1] * matrix[1][2])
                    - matrix[1][0] * (matrix[0][1] * matrix[2][2] - matrix[2][1] * matrix[0][2])
                    + matrix[2][0] * (matrix[0][1] * matrix[1][2] - matrix[1][1] * matrix[0][2]);

        Mat3x3 result;
        result.matrix[0][0] = (matrix[1][1] * matrix[2][2] - matrix[2][1] * matrix[1][2]) * (1 / det);
        result.matrix[1][0] = -(matrix[1][0] * matrix[2][2] - matrix[2][0] * matrix[1][2]) * (1 / det);
        result.matrix[2][0] = (matrix[1][0] * matrix[2][1] - matrix[2][0] * matrix[1][1]) * (1 / det);
        result.matrix[0][1] = -(matrix[0][1] * matrix[2][2] - matrix[2][1] * matrix[0][2]) * (1 / det);
        result.matrix[1][1] = (matrix[0][0] * matrix[2][2] - matrix[2][0] * matrix[0][2]) * (1 / det);
        result.matrix[2][1] = -(matrix[0][0] * matrix[2][1] - matrix[2][0] * matrix[0][1]) * (1 / det);
        result.matrix[0][2] = (matrix[0][1] * matrix[1][2] - matrix[1][1] * matrix[0][2]) * (1 / det);
        result.matrix[1][2] = -(matrix[0][0] * matrix[1][2] - matrix[1][0] * matrix[0][2]) * (1 / det);
        result.matrix[2][2] = (matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1]) * (1 / det);

        return result;
    };

    Mat3x3 transpose() {
        return Mat3x3(Column(0), Column(1), Column(2));
    };

    Vec3 Row(int index) {
        if (index >= 0 && index < 3) {
            return {matrix[index][0], matrix[index][1], matrix[index][2]};
        }
        return Vec3(0.0f);
    };

    Vec3 Column(int index) {
        if (index >= 0 && index < 3) {
            float res[3]{};
            for (int i = 0; i < 3; i++) {
                res[i] = matrix[i][index];
            }
            return {res[0], res[1], res[2]};
        }
        return Vec3(0.0f);
    };

    float *operator[](int row) {
        return matrix[row];
    }

    bool operator==(const Mat3x3 &other) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (matrix[i][j] != other.matrix[i][j])
                    return false;
        return true;
    };

    bool operator!=(const Mat3x3 &other) {
        return matrix != other.matrix;
    };

    float matrix[3][3]{};
};

#endif //CG_ENGINE_MAT3X3_H
