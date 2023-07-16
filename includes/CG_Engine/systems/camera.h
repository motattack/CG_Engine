#ifndef CG_ENGINE_CAMERA_SYSTEM_H
#define CG_ENGINE_CAMERA_SYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/camera.h>
#include <CG_Engine/engine.h>
#include <CG_Engine/ui/input.h>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/source.h>

class CameraSystem : public BaseSystem {

public:
    void start() override {
        addComponentSignature<Camera>();
        addComponentSignature<Transform>();

        shader = Shader(Resource.program("MAIN"));
        skyboxShader = Shader(Resource.program("SKYBOX"));
    }

    void update() override {
        for (auto entity: entities) {
            auto &camera = Manager.getComponent<Camera>(entity);
            auto &transform = Manager.getComponent<Transform>(entity);
            onZoom(camera);
            camera.Position = transform.Position;
            camera.Right = transform.Rotation;

            skyboxShader.bind();
            skyboxShader.setMat4("projection", camera.GetProjectionMatrix());
            skyboxShader.setMat4("view", camera.GetViewMatrixClear());
            glBindTexture(GL_TEXTURE_CUBE_MAP, Resource.cubeMap("SKYBOX"));
            Resource.getMesh("CUBE").drawArrays(skyboxShader);

            shader.bind();
            shader.setMat4("projection", camera.GetProjectionMatrix());
            shader.setMat4("view", camera.GetViewMatrix());
        }
    }

    void onZoom(Camera &camera) {
        auto &event = Input::Event();
        if (event.isMouseScrolling()) {
            camera.ChangeFOV(event.mouseScrollOffset().y);
        }
        event.resetScrollState();
    }

private:
    Shader shader;
    Shader skyboxShader;
};


#endif //CG_ENGINE_CAMERA_SYSTEM_H
