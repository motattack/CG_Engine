#ifndef CG_ENGINE_MAT4X4_H
#define CG_ENGINE_MAT4X4_H

#include <cmath>
#include "vec3.h"
#include "vec4.h"
#include "mat3x3.h"
#include <vector>
#include <cassert>

class Mat4x4 {
public:
    Mat4x4() = default;

    explicit Mat4x4(float number) : matrix{{number, 0,      0,      0},
                                           {0,      number, 0,      0},
                                           {0,      0,      number, 0},
                                           {0,      0,      0,      number}} {};

    Mat4x4(float a, float b, float c, float d,
           float e, float f, float g, float h,
           float i, float j, float k, float l,
           float m, float n, float o, float r) : matrix{{a, b, c, d},
                                                        {e, f, g, h},
                                                        {i, j, k, l},
                                                        {m, n, o, r}} {};


    Mat4x4(const Vec4 &first, const Vec4 &second, const Vec4 &third, const Vec4 &four)
            : matrix{{first.x,  first.y,  first.z,  first.w},
                     {second.x, second.y, second.z, second.w},
                     {third.x,  third.y,  third.z,  third.w},
                     {four.x,   four.y,   four.z,   four.w}} {}

    Mat4x4(const Mat4x4 &other) = default;

    Mat4x4 operator+(const Mat4x4 &other) const {
        Mat4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result[i][j] = matrix[i][j] + other.matrix[i][j];
        return result;
    }

    Mat4x4 &operator+=(const Mat4x4 &other) {
        *this = *this + other;
        return *this;
    }

    Mat4x4 operator-(const Mat4x4 &other) const {
        Mat4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result[i][j] = matrix[i][j] - other.matrix[i][j];
        return result;
    }

    Mat4x4 &operator-=(const Mat4x4 &other) {
        *this = *this - other;
        return *this;
    }

    Mat4x4 operator*(const Mat4x4 &other) const {
        Mat4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                result[i][j] = 0;
                for (int k = 0; k < 4; k++)
                    result[i][j] += other.matrix[i][k] * matrix[k][j];
            }
        return result;
    }

    Mat4x4 &operator*=(const Mat4x4 &other) {
        *this = *this * other;
        return *this;
    }

    Mat4x4 operator*(float number) const {
        Mat4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result[i][j] = matrix[i][j] * number;
        return result;
    }

    Mat4x4 &operator*=(float number) {
        *this = *this * number;
        return *this;
    }

    Mat4x4 operator/(float number) const {
        Mat4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result[i][j] = matrix[i][j] / number;
        return result;
    }

    Mat4x4 &operator/=(float number) {
        *this = *this / number;
        return *this;
    }

    Mat4x4 operator+(float number) const {
        Mat4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result[i][j] = matrix[i][j] + number;
        return result;
    }

    Mat4x4 &operator+=(float number) {
        *this = *this + number;
        return *this;
    }

    Mat4x4 operator-(float number) const {
        Mat4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result[i][j] = matrix[i][j] - number;
        return result;
    }

    Mat4x4 &operator-=(float number) {
        *this = *this - number;
        return *this;
    }

    Vec4 operator*(const Vec4 &vec) const {
        Vec4 result;
        result.x = matrix[0][0] * vec.x + matrix[1][0] * vec.y + matrix[2][0] * vec.z + matrix[3][0] * vec.w;
        result.y = matrix[0][1] * vec.x + matrix[1][1] * vec.y + matrix[2][1] * vec.z + matrix[3][1] * vec.w;
        result.z = matrix[0][2] * vec.x + matrix[1][2] * vec.y + matrix[2][2] * vec.z + matrix[3][2] * vec.w;
        result.w = matrix[0][3] * vec.x + matrix[1][3] * vec.y + matrix[2][3] * vec.z + matrix[3][3] * vec.w;
        return result;
    }

    static Mat4x4 identity() {
        return Mat4x4(1.0f);
    };

    float det() {
        Mat3x3 A(matrix[1][1], matrix[1][2], matrix[1][3], matrix[2][1], matrix[2][2], matrix[2][3], matrix[3][1],
                 matrix[3][2], matrix[3][3]);
        Mat3x3 B(matrix[0][1], matrix[0][2], matrix[0][3], matrix[2][1], matrix[2][2], matrix[2][3], matrix[3][1],
                 matrix[3][2], matrix[3][3]);
        Mat3x3 C(matrix[0][1], matrix[0][2], matrix[0][3], matrix[1][1], matrix[1][2], matrix[1][3], matrix[3][1],
                 matrix[3][2], matrix[3][2]);
        Mat3x3 D(matrix[0][1], matrix[0][2], matrix[0][3], matrix[1][1], matrix[1][2], matrix[1][3], matrix[2][1],
                 matrix[2][2], matrix[2][3]);

        return matrix[0][0] * A.det() - matrix[1][0] * B.det() + matrix[2][0] * C.det() - matrix[3][0] * D.det();
    }

    Mat4x4 reverse() {
        Mat4x4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Mat3x3 subMatrix = getSubMatrix(i, j);
                float sign = (i + j) % 2 == 0 ? 1 : -1;
                float cofactor = subMatrix.det() * sign;
                result[j][i] = cofactor / det();
            }
        }
        return result.transpose();
    }

    Mat3x3 getSubMatrix(int exclude_row, int exclude_col) const {
        Mat3x3 subMatrix;
        int row = 0, col = 0;
        for (int i = 0; i < 4; ++i) {
            if (i == exclude_row) continue;
            for (int j = 0; j < 4; ++j) {
                if (j == exclude_col) continue;
                subMatrix[row][col] = matrix[i][j];
                ++col;
            }
            ++row;
            col = 0;
        }
        return subMatrix;
    }

    Mat4x4 transpose() {
        return Mat4x4(Row(0), Row(1), Row(2), Row(3));
    };

    Mat4x4 translate(const Vec3 &axis) {
        Mat4x4 translated_matrix(*this);

        translated_matrix[3][0] = matrix[0][0] * axis.x + matrix[1][0] * axis.y + matrix[2][0] * axis.z + matrix[3][0];
        translated_matrix[3][1] = matrix[0][1] * axis.x + matrix[1][1] * axis.y + matrix[2][1] * axis.z + matrix[3][1];
        translated_matrix[3][2] = matrix[0][2] * axis.x + matrix[1][2] * axis.y + matrix[2][2] * axis.z + matrix[3][2];
        translated_matrix[3][3] = matrix[0][3] * axis.x + matrix[1][3] * axis.y + matrix[2][3] * axis.z + matrix[3][3];

        return translated_matrix;
    }

    [[nodiscard]] Mat4x4 rotate(float angle, Vec3 axis) const {
        // Вычисляем синус и косинус угла поворота
        float cos_angle = cosf(angle);
        float sin_angle = sinf(angle);

        Vec3 normalized_axis = axis.normalize(); // Нормализуем вектор оси вращения
        Vec3 sup = normalized_axis * (1 - cos_angle); // Вычисляем вспомогательный вектор

        Mat4x4 rotation_matrix;

        rotation_matrix[0][0] = cos_angle + sup.x * normalized_axis.x;
        rotation_matrix[0][1] = sup.x * normalized_axis.y + sin_angle * normalized_axis.z;
        rotation_matrix[0][2] = sup.x * normalized_axis.z - sin_angle * normalized_axis.y;
        rotation_matrix[0][3] = 0;

        rotation_matrix[1][0] = sup.y * normalized_axis.x - sin_angle * normalized_axis.z;
        rotation_matrix[1][1] = cos_angle + sup.y * normalized_axis.y;
        rotation_matrix[1][2] = sup.y * normalized_axis.z + sin_angle * normalized_axis.x;
        rotation_matrix[1][3] = 0;

        rotation_matrix[2][0] = sup.z * normalized_axis.x + sin_angle * normalized_axis.y;
        rotation_matrix[2][1] = sup.z * normalized_axis.y - sin_angle * normalized_axis.x;
        rotation_matrix[2][2] = cos_angle + sup.z * normalized_axis.z;
        rotation_matrix[2][3] = 0;

        rotation_matrix[3][0] = 0;
        rotation_matrix[3][1] = 0;
        rotation_matrix[3][2] = 0;
        rotation_matrix[3][3] = 1;

        return Mat4x4(*this * rotation_matrix);
    }

    Mat4x4 Scale(const Vec3 &vec) {
        Mat4x4 result(*this);

        result[0][0] *= vec.x;
        result[0][1] *= vec.x;
        result[0][2] *= vec.x;
        result[0][3] *= vec.x;

        result[1][0] *= vec.y;
        result[1][1] *= vec.y;
        result[1][2] *= vec.y;
        result[1][3] *= vec.y;

        result[2][0] *= vec.z;
        result[2][1] *= vec.z;
        result[2][2] *= vec.z;
        result[2][3] *= vec.z;

        return result;
    }

    Vec4 Row(int index) {
        if (index >= 0 && index < 4) {
            float result[4]{};
            for (int i = 0; i < 4; i++) {
                result[i] = matrix[i][index];
            }
            return {result[0], result[1], result[2], result[3]};
        }
        return Vec4(0.0f);
    };

    Vec4 Column(int index) {
        if (index >= 0 && index < 4) {
            Vec4 result(matrix[index][0], matrix[index][1], matrix[index][2], matrix[index][3]);
            return result;
        }
        return Vec4(0.0f);
    };

    static Mat4x4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
        Vec3 direction((center - eye).normalize()); // Вычисляем вектор направления камеры

        Vec3 right(direction.crossProduct(up).normalize()); // Вычисляем вектор направления вправо от камеры

        Vec3 cameraUp(right.crossProduct(direction)); // Вычисляем вектор направления вверх от камеры

        Mat4x4 result(1.0f);

        result[0][0] = right.x;
        result[1][0] = right.y;
        result[2][0] = right.z;
        result[0][1] = cameraUp.x;
        result[1][1] = cameraUp.y;
        result[2][1] = cameraUp.z;
        result[0][2] = -direction.x;
        result[1][2] = -direction.y;
        result[2][2] = -direction.z;
        result[3][0] = -right.dotProduct(eye);
        result[3][1] = -cameraUp.dotProduct(eye);
        result[3][2] = direction.dotProduct(eye);

        return result;
    }

    static Mat4x4 perspective(float fovy, float aspect, float zNear, float zFar) {
        assert(abs(aspect - std::numeric_limits<float>::epsilon()) > static_cast<float>(0)); // Проверка деления на 0

        float tanHalfFovy = tanf(fovy / 2.0f); // Вычисление тангенса половины угла обзора

        Mat4x4 result(0.0f);

        // Расчет элементов матрицы проекции
        result[0][0] = 1.0f / (aspect * tanHalfFovy);
        result[1][1] = 1.0f / (tanHalfFovy);
        result[2][2] = -(zFar + zNear) / (zFar - zNear);
        result[2][3] = -1.0f;
        result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);

        return result;
    }

    static Mat4x4 ortho(float left, float right, float bottom, float top) {
        Mat4x4 result(1.0f);
        result[0][0] = 2.0f / (right - left);
        result[1][1] = 2.0f / (top - bottom);
        result[2][2] = -1.0f;
        result[3][0] = -(right + left) / (right - left);
        result[3][1] = -(top + bottom) / (top - bottom);
        return result;
    }

    float *operator[](int row) {
        return matrix[row];
    }

    bool operator==(const Mat4x4 &other) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (matrix[i][j] != other.matrix[i][j])
                    return false;
        return true;
    };

    bool operator!=(const Mat4x4 &other) {
        return matrix != other.matrix;
    };

    float matrix[4][4]{};
};

#endif //CG_ENGINE_MAT4X4_H
