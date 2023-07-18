#ifndef CG_ENGINE_SPRITERENDERERSYSTEM_H
#define CG_ENGINE_SPRITERENDERERSYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/spriteRenderer.h>
#include <CG_Engine/engine.h>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/source.h>
#include <>

class SpriteRendererSystem : public BaseSystem {
public:
    SpriteRendererSystem() {
        addComponentSignature<Transform>();
        addComponentSignature<SpriteRenderer>();
        shader = Shader(Resource.program("SPRITE"));
    }

    void render() override {
        for (auto &entity: entities) {
            Manager.getComponent<Transform>(entity).setModelUniform(shader);
            Manager.getComponent<SpriteRenderer>(entity).SetUniform(shader);
            shader.bind();
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            quad.VAO.DrawElements(GL_TRIANGLES);
            glDisable(GL_BLEND);
        }
    }

private:
    GLQuad quad;
    Shader shader;
};

#endif //CG_ENGINE_SPRITERENDERERSYSTEM_H
