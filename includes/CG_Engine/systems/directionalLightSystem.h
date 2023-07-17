#ifndef CG_ENGINE_DIRECTIONALLIGHTSYSTEM_H
#define CG_ENGINE_DIRECTIONALLIGHTSYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/directionalLight.h>
#include <CG_Engine/engine.h>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/source.h>

class DirectionalLightSystem : public BaseSystem {

public:
    DirectionalLightSystem() {
        addComponentSignature<Transform>();
        addComponentSignature<DirectionalLight>();

        shader = Shader(Resource.program("MAIN"));
    }

    void update() override {
        for (auto entity: entities) {
            auto &light = Manager.getComponent<DirectionalLight>(entity);
            auto &transform = Manager.getComponent<Transform>(entity);

            shader.bind();
            shader.setFloat("material.shininess", 64.0f);
            light.light.direction = transform.Position;
            light.light.render(shader, 0);
        }
    }

private:
    Shader shader;
};

#endif //CG_ENGINE_DIRECTIONALLIGHTSYSTEM_H
