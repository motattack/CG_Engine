#ifndef CG_ENGINE_POINTLIGHTSYSTEM_H
#define CG_ENGINE_POINTLIGHTSYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/pointLight.h>
#include <CG_Engine/engine.h>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/source.h>

class PointLightSystem : public BaseSystem {

public:
    PointLightSystem() {
        addComponentSignature<Transform>();
        addComponentSignature<PointLighting>();

        shader = Shader(Resource.program("MAIN"));
    }

    void update() {
        for (auto entity: entities) {
            auto &light = Manager.getComponent<PointLighting>(entity);
            auto &transform = Manager.getComponent<Transform>(entity);
            shader.bind();
            light.light.position = transform.Position;
            for (int i = 0; i < 4; i++) {
                light.light.render(shader, i);
            }
        }
    }
private:
    Shader shader;
};

#endif //CG_ENGINE_POINTLIGHTSYSTEM_H
