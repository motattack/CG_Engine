#ifndef CG_ENGINE_CAMERA_SYSTEM_H
#define CG_ENGINE_CAMERA_SYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/camera.h>
#include <CG_Engine/engine.h>
#include <CG_Engine/ui/input.h>
#include <CG_Engine/render/shader.h>

class CameraSystem : public BaseSystem {

public:
    void start() override {
        addComponentSignature<Camera>();
        addComponentSignature<Transform>();

        float width = core.videoWidth();
        float height = core.videoHeight();

        glViewport(0, 0, width, height);
        std::cout << width << "x" << height << std::endl;

        shader = Shader("res/shader/vShader.glsl", "res/shader/fShader.glsl");
        skyboxShader = Shader("res/shader/sbox.vert", "res/shader/sbox.frag");
    }

    void update() override {
        for (auto entity: entities) {
            auto &camera = Manager.getComponent<Camera>(entity);
            auto &transform = Manager.getComponent<Transform>(entity);
            onZoom(camera);
            std::cout << camera.Zoom << std::endl;

            shader.use();
            shader.set3Float("model", transform.Position);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);

            skyboxShader.use();
            skyboxShader.setMat4("projection", Mat4x4(1));
            skyboxShader.setMat4("view", Mat4x4(1));
        }
    }

    void onDrag(Camera &camera) {
        auto &event = Input::Event();
        if (event.isMouseMoving() && event.isMouseDown(MouseBtn::LEFT)) {
            Vec2 offset = event.mouseMotionOffset() * camera.Sensitivity;

            camera.Yaw += offset.x;
            camera.Pitch += offset.y;

            if (camera.ContrainPitch) {
                if (camera.Pitch > MAX_PITCH) {
                    camera.Pitch = MAX_PITCH;
                }

                if (camera.Pitch < -MAX_PITCH) {
                    camera.Pitch = -MAX_PITCH;
                }
            }

            Vec3 front(0.0f);
            front.x = cosf(radians(camera.Yaw)) * cosf(radians(camera.Pitch));
            front.y = sinf(radians(camera.Pitch));
            front.z = sinf(radians(camera.Yaw)) * cosf(radians(camera.Pitch));
            camera.Front = front.normalize();

            camera.Right = camera.Front.crossProduct(camera.WorldUp).normalize();
            camera.Up = camera.Right.crossProduct(camera.Right).normalize();
        }
    }

    void onZoom(Camera &camera) {
        auto &event = Input::Event();
        if (event.isMouseScrolling()) {
            camera.Zoom += event.mouseScrollOffset().y * camera.ScrollStep;
            camera.Zoom = (camera.Zoom <= 1.0f) ? 1.0f : camera.Zoom;
            camera.Zoom = (camera.Zoom >= ZOOM) ? ZOOM : camera.Zoom;
        }
        event.resetScrollState();
    }

private:
    Shader shader, skyboxShader;
};


#endif //CG_ENGINE_CAMERA_SYSTEM_H
