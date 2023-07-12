#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <CG_Engine/math/common.h>

enum class CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW = 90.0f; // лево-право
const float PITCH = 0.0f; // вперёд-назад

const float SPEED = 1.0f;
const float SENSITIVITY = 0.05f;

const float FOV = 45.0f; // обзор
const float ZNEAR = 0.01f; // угол отсечения
const float ZFAR = 1000.f; // .
const float ASPECTRATIO = 16.f / 9.f; // соотношение сторон


class Camera {
public:
    Vec3 Position;
    Vec3 Front;
    Vec3 Up;
    Vec3 Right;
    Vec3 WorldUp;

    float Yaw;
    float Pitch;

    float Fov;
    float zNear;
    float zFar;
    float AspectRatio;

    float MovementSpeed;
    float Sensitivity;

    explicit Camera(Vec3 position = Vec3(0.0f, 0.0f, 0.0f),
                    Vec3 up = Vec3(0.0f, 1.0f, 0.0f),
                    float yaw = YAW, float pitch = PITCH) :
            Front(Vec3(0.0f, 0.0f, -1.0f)),
            Fov(FOV), zNear(ZNEAR), zFar(ZFAR), AspectRatio(ASPECTRATIO),
            MovementSpeed(SPEED), Sensitivity(SENSITIVITY) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
    };

    Camera(float posX, float posY, float posZ,
           float upX, float upY, float upZ,
           float yaw, float pitch) :
            Front(Vec3(0.0f, 0.0f, -1.0f)),
            Fov(FOV), zNear(ZNEAR), zFar(ZFAR), AspectRatio(ASPECTRATIO),
            MovementSpeed(SPEED), Sensitivity(SENSITIVITY) {
        Position = Vec3(posX, posY, posZ);
        WorldUp = Vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
    };

    Mat4x4 GetViewMatrix() const {
        return Mat4x4::lookAt(Position, Position + Front, Up);
    };

    Mat4x4 GetViewMatrixClear() const {
        Mat4x4 result = Mat4x4::lookAt(Position, Position + Front, Up);
        for (int i = 0; i < 4; i++) {
            result[3][i] = result[i][3] = 0;
        }
        result[3][3] = 1;
        return result;
    };

    Mat4x4 GetProjectionMatrix() const {
        return Mat4x4::perspective(radians(Fov), AspectRatio, zNear, zFar);
    };

//    void Move(double dx, double dy) {
//        Yaw += dx;
//        Pitch += dy;
//
//        if (Pitch > 89.0f) {
//            Pitch = 89.0f;
//        }
//        if (Pitch < -89.0f) {
//            Pitch = -89.0f;
//        }
//
//        UpdateCameraVectors();
//    };

    void Rotate(float xoffset, float yoffset) {
        xoffset *= Sensitivity * MovementSpeed;
        yoffset *= Sensitivity * MovementSpeed;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        UpdateCameraVectors();
    };

    void ChangeFOV(double value) {
        Fov -= (float) value;
        if (Fov < 1.0f)
            Fov = 1.0f;
        if (Fov > 120.0f)
            Fov = 120.0f;
        UpdateCameraVectors();
    };

    void Move(CameraDirection direction, double dt) {
        float velocity = (float) dt * MovementSpeed;

        switch (direction) {
            case CameraDirection::FORWARD:
                Position += Front * velocity;
                break;
            case CameraDirection::BACKWARD:
                Position -= Front * velocity;
                break;
            case CameraDirection::RIGHT:
                Position += Right * velocity;
                break;
            case CameraDirection::LEFT:
                Position -= Right * velocity;
                break;
            case CameraDirection::UP:
                Position += Up * velocity;
                break;
            case CameraDirection::DOWN:
                Position -= Up * velocity;
                break;
            case CameraDirection::NONE:
                break;
        }
    };

    void UpdateCameraVectors() {
        Vec3 front;
        front.x = cosf(radians(Yaw)) * cosf(radians(Pitch));
        front.y = sinf(radians(Pitch));
        front.z = sinf(radians(Yaw)) * cosf(radians(Pitch));
        Front = front.normalize();

        Right = Front.crossProduct(WorldUp).normalize();
        Up = Right.crossProduct(Front).normalize();
    };
};

#endif