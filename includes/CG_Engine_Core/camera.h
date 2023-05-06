#ifndef CG_ENGINE_CAMERA_H
#define CG_ENGINE_CAMERA_H

#include <GL/glew.h>
#include <vector>
#include <CG_Engine_Core/math/vec3.h>
#include <CG_Engine_Core/math/mat4x4.h>
#include <CG_Engine_Core/math/common.h>

enum Camera_Movement {
    FORWARD, BACKWARD,
    LEFT, RIGHT,
    UP, DOWN
};

const float YAW = -90.0f;
const float PITCH = -0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
private:
    void updateCameraVectors() {
        Vec3 direction;
        direction.x = std::cos(radians(this->Yaw)) * std::cos(radians(this->Pitch));
        direction.y = std::sin(radians(this->Pitch));
        direction.z = std::sin(radians(this->Yaw)) * std::cos(radians(this->Pitch));
        this->Front = direction.normalize();

        // Also re-calculate for Right and Up vectors
        this->Right = this->Front.crossProduct(this->WorldUp).normalize();
        this->Up = this->Right.crossProduct(this->Front).normalize();
    }

public:
    Vec3 Position;
    Vec3 Front, Up;
    Vec3 WorldUp;
    Vec3 Right;

    // Euler Angles
    float Yaw;
    float Pitch;

    //Camera Options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    explicit Camera(Vec3 position = Vec3(0.0f), Vec3 up = Vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
            : Front(Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        updateCameraVectors();
    }

    Mat4x4 GetViewMatrix() const {
        return Mat4x4::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

    void ProcessMouseMovement(float xoffset, float yoffset) {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset;
        this->Pitch += yoffset;

        if (this->Pitch >= 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch <= -89.0f)
            this->Pitch = -89.0f;

        updateCameraVectors();
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        const float velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * velocity;
        if (direction == UP)
            this->Position += this->Up * velocity;
        if (direction == DOWN)
            this->Position -= this->Up * velocity;
    }

    void ProcessMouseScroll(float yoffset) {
        this->Zoom -= float(yoffset);
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
        if (Zoom <= 20.0f)
            Zoom = 20.0f;
    }

    void mouseCursorPosition(const sf::Vector2i pos, sf::Window &window) {
        sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);
        sf::Mouse::setPosition(center, window);

        float xpos = pos.x;
        float ypos = pos.y;

        float xoffset = xpos - center.x;
        float yoffset = center.y - ypos;

        ProcessMouseMovement(xoffset, yoffset);
    }

    void mouseScrollCallback(const sf::Event &event) {
        ProcessMouseScroll(event.mouseWheelScroll.delta);
    }
};

#endif //CG_ENGINE_CAMERA_H
