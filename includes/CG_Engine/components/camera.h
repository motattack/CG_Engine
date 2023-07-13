#ifndef CG_ENGINE_CAMERA_H
#define CG_ENGINE_CAMERA_H

#include <CG_Engine/base/baseComponent.h>
#include <CG_Engine/math/common.h>

const float YAW = -90.0f;
const float ZOOM = 45.0f;
const float PITCH = 0.0f;
const float MAX_PITCH = 89.0f;
const float SENSITIVTY = 0.04f;

struct Camera : public BaseComponent {

    Camera(): Front(0.0f, 0.0f, -1.0f), Up(0.0f, 1.0f, 0.0f),
              Right(1.0f, 0.0f, 0.0f), WorldUp(0.0f, 1.0f, 0.0f),Pitch(PITCH), Yaw(YAW),
              Sensitivity(SENSITIVTY), Zoom(ZOOM), ScrollStep(0.1f), ContrainPitch(true) { }

    ~Camera() = default;

    // camera vectors
    Vec3 WorldUp;
    Vec3 Front;
    Vec3 Right;
    Vec3 Up;

    // Eular angles
    float Yaw;
    float Pitch;

    // motion
    float Zoom;
    float ScrollStep;
    float Sensitivity;
    bool ContrainPitch;
};


#endif //CG_ENGINE_CAMERA_H
