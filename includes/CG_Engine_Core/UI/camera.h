#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>

#include "CG_Engine_Core/math/vec3.h"
#include "CG_Engine_Core/math/mat4x4.h"
#include "CG_Engine_Core/math/common.h"
#include "CG_Engine_Core/physics/evn.h"

/*
    enum to represent directions for movement
*/

// t

enum class CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

/*
    camera class to help display from POV of camera
*/

class Camera {
public:
    /*
        camera values
    */

    // position
    Vec3 cameraPos;

    // camera directional values
    Vec3 cameraFront;
    Vec3 cameraUp;
    Vec3 cameraRight;

    // camera rotational values
    float yaw; // x-axis
    float pitch; // y-axis

    // camera movement values
    float speed;
    float sensitivity;
    float zoom;

    /*
        constructor
    */

    // default and initialize with position
    Camera(Vec3 position = Vec3(0.0f)) : cameraPos(position),
                                         yaw(0.0f),
                                         pitch(0.0f),
                                         speed(2.5f),
                                         sensitivity(1.0f),
                                         zoom(45.0f),
                                         cameraFront(Vec3(1.0f, 0.0f, 0.0f)) {
        updateCameraVectors();
    };

    /*
        modifiers
    */

    // change camera direction (mouse movement)
    void updateCameraDirection(double dx, double dy) {
        yaw += dx;
        pitch += dy;

        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }

        updateCameraVectors();
    };

    // change camera position in certain direction (keyboard)
    void updateCameraPos(CameraDirection direction, double dt) {
        float velocity = (float) dt * speed;

        switch (direction) {
            case CameraDirection::FORWARD:
                cameraPos += cameraFront * velocity;
                break;
            case CameraDirection::BACKWARD:
                cameraPos -= cameraFront * velocity;
                break;
            case CameraDirection::RIGHT:
                cameraPos += cameraRight * velocity;
                break;
            case CameraDirection::LEFT:
                cameraPos -= cameraRight * velocity;
                break;
            case CameraDirection::UP:
                cameraPos += cameraUp * velocity;
                break;
            case CameraDirection::DOWN:
                cameraPos -= cameraUp * velocity;
                break;
        }
    };

    // change camera zoom (scroll wheel)
    void updateCameraZoom(double dy) {
        if (zoom >= 1.0f && zoom <= 45.0f) {
            zoom -= dy;
        } else if (zoom < 1.0f) {
            zoom = 1.0f;
        } else { // > 45.0f
            zoom = 45.0f;
        }
    };

    /*
        accessors
    */

    // get view matrix for camera
    Mat4x4 getViewMatrix() {
        return Mat4x4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    };

    // get zoom value for camera
    float getZoom() {
        return zoom;
    };

private:
    /*
        private modifier
    */

    // change camera directional vectors based on movement
    void updateCameraVectors() {
        Vec3 direction;
        direction.x = std::cos(radians(yaw)) * std::cos(radians(pitch));
        direction.y = std::sin(radians(pitch));
        direction.z = std::sin(radians(yaw)) * std::cos(radians(pitch));
        cameraFront = direction.normalize();

        cameraRight = (cameraFront.crossProduct(Environment::worldUp)).normalize();
        cameraUp = cameraRight.crossProduct(cameraFront).normalize();
    };
};

#endif