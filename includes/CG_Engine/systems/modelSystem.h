#ifndef CG_ENGINE_MODELSYSTEM_H
#define CG_ENGINE_MODELSYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/source.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/modelRenderer.h>
#include <CG_Engine/engine.h>

class ModelRendererSystem : public BaseSystem {

public:
    ModelRendererSystem() {
        addComponentSignature<Transform>();
        addComponentSignature<ModelRenderer>();

        shader = Shader(Resource.program("MAIN"));
    }

    void render() override {
        shader.bind();
        for (auto entity: entities) {
            Manager.getComponent<Transform>(entity).setModelUniform(shader);
            Manager.getComponent<ModelRenderer>(entity).Renderer->Draw(shader);
        }
        shader.unBind();
    }

private:
    Shader shader;
};

#endif //CG_ENGINE_MODELSYSTEM_H
