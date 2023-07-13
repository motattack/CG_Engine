#ifndef CG_ENGINE_CAMERA_H
#define CG_ENGINE_CAMERA_H

#include <CG_Engine/base/baseComponent.h>
#include <CG_Engine/math/common.h>

const float YAW = 90.0f; // лево-право
const float PITCH = 0.0f; // вперёд-назад

const float SPEED = 1.0f;
const float SENSITIVITY = 0.05f;

const float FOV = 45.0f; // обзор
const float ZNEAR = 0.01f; // угол отсечения
const float ZFAR = 1000.f; // .
const float ASPECTRATIO = 16.f / 9.f; // соотношение сторон

struct Camera : public BaseComponent {

    Camera(Vec3 up = Vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH) :
            Front(Vec3(0.0f, 0.0f, -1.0f)),
            Fov(FOV), zNear(ZNEAR), zFar(ZFAR), AspectRatio(ASPECTRATIO),
            MovementSpeed(SPEED), Sensitivity(SENSITIVITY),
            Yaw(yaw), Pitch(pitch), WorldUp(up) {}

    ~Camera() = default;

    Vec3 WorldUp;
    Vec3 Front;
    Vec3 Right;
    Vec3 Up;

    float Yaw;
    float Pitch;

    float Fov;
    float zNear;
    float zFar;
    float AspectRatio;

    float MovementSpeed;
    float Sensitivity;
};


#endif //CG_ENGINE_CAMERA_H
