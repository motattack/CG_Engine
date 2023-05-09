#ifndef CAMERA_H
#define CAMERA_H

#include "CG_Engine_Core/math/vec3.h"
#include "CG_Engine_Core/math/mat4x4.h"
#include "CG_Engine_Core/math/common.h"

enum class CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    static Camera defaultCamera;

    Vec3 cameraPos;

    Vec3 cameraFront;
    Vec3 cameraUp;
    Vec3 cameraRight;

    Vec3 worldUp;

    float yaw; // x-axis
    float pitch; // y-axis
    float speed;
    float sensitivity;
    float zoom;

    explicit Camera(Vec3 position = Vec3(0.0f))
            : cameraPos(position),
              worldUp(Vec3(0.0f, 1.0f, 0.0f)),
              yaw(0.0f),
              pitch(0.0f),
              speed(2.5f),
              sensitivity(1.0f),
              zoom(45.0f),
              cameraFront(Vec3(1.0f, 0.0f, 0.0f)) {
        updateCameraVectors();
    };

    void mouseCursorPosition(const sf::Vector2i pos, sf::Window &window) {
        sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);
        sf::Mouse::setPosition(center, window);

        float xpos = pos.x;
        float ypos = pos.y;

        float xoffset = xpos - center.x;
        float yoffset = center.y - ypos;

        updateCameraDirection(xoffset, yoffset);
    }

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
    }; // moving mouse
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
    }; // keyboard input
    void updateCameraZoom(double dy) {
        if (zoom >= 1.0f && zoom <= 45.0f) {
            zoom -= dy;
        } else if (zoom < 1.0f) {
            zoom = 1.0f;
        } else { // > 45.0f
            zoom = 45.0f;
        }
    }; // scroll wheel

    [[nodiscard]] Mat4x4 getViewMatrix() const {
        return Mat4x4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    };

    [[nodiscard]] float getZoom() const {
        return zoom;
    };

private:
    void updateCameraVectors() {
        Vec3 direction;
        direction.x = cosf(radians(yaw)) * cosf(radians(pitch));
        direction.y = sinf(radians(pitch));
        direction.z = sinf(radians(yaw)) * cosf(radians(pitch));
        cameraFront = direction.normalize();

        cameraRight = cameraFront.crossProduct(worldUp).normalize();
        cameraUp = cameraRight.crossProduct(cameraFront).normalize();
    };
};

#endif