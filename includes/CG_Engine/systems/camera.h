#ifndef CG_ENGINE_CAMERA_SYSTEM_H
#define CG_ENGINE_CAMERA_SYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/camera.h>
#include <CG_Engine/engine.h>

class CameraSystem : public BaseSystem {

public:
    void start() override {
        addComponentSignature<Camera>();
        float width = core.videoWidth();
        float height = core.videoHeight();

        glViewport(0, 0, width, height);
        std::cout << width << "x" << height << std::endl;
    }

    void update() override {
        for (auto entity: entities) {
            auto &camera = Manager.getComponent<Camera>(entity);
            OnZoom(camera);
        }
    }

    void OnZoom(Camera &camera) {
//        std::cout << camera.Zoom << std::endl;
    }

private:
    Shader shader, skyboxShader;
};


#endif //CG_ENGINE_CAMERA_SYSTEM_H
