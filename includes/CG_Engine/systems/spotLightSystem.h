#ifndef CG_ENGINE_SPOTLIGHTSYSTEM_H
#define CG_ENGINE_SPOTLIGHTSYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/spotLight.h>
#include <CG_Engine/engine.h>
#include <CG_Engine/ui/input.h>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/source.h>

class SpotLightSystem : public BaseSystem {

public:
    SpotLightSystem() {
        addComponentSignature<Transform>();
        addComponentSignature<SpotLighting>();

        shader = Shader(Resource.program("MAIN"));
    }

    void update() {
        for (auto entity: entities) {
            auto &light = Manager.getComponent<SpotLighting>(entity);
            auto &transform = Manager.getComponent<Transform>(entity);
            shader.bind();
            light.light.position = transform.Position;
            light.light.render(shader, 0);
        }
    }

private:
    Shader shader;
};

#endif //CG_ENGINE_SPOTLIGHTSYSTEM_H
