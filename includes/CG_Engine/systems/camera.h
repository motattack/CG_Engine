#ifndef CG_ENGINE_CAMERA_SYSTEM_H
#define CG_ENGINE_CAMERA_SYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/camera.h>
#include <CG_Engine/engine.h>

class CameraSystem : public BaseSystem {

public:
    void start() override {
        addComponentSignature<Transform>();
        addComponentSignature<Camera>();
        float width = core.videoWidth();
        std::cout << width;
    }

private:
    Shader shader, skyboxShader;
};


#endif //CG_ENGINE_CAMERA_SYSTEM_H
