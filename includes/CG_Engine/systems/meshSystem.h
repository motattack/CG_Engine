#ifndef CG_ENGINE_MESHSYSTEM_H
#define CG_ENGINE_MESHSYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/meshRenderer.h>
#include <CG_Engine/engine.h>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/source.h>

class MeshRendererSystem : public BaseSystem {

public:
    MeshRendererSystem() {
        addComponentSignature<Transform>();
        addComponentSignature<MeshRenderer>();
        shader = Shader(Resource.program("MAIN"));
    }

    void render() override {
        for (auto entity: entities) {
            auto &renderer = Manager.getComponent<MeshRenderer>(entity);
            auto &transform = Manager.getComponent<Transform>(entity);

            unsigned int diffuseMap = Resource.Texture2D("bricks");
            unsigned int normalMap = Resource.Texture2D("bricks2_disp");
            unsigned int heightMap = Resource.Texture2D("bricks2_normal");

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalMap);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, heightMap);

            transform.setModelUniform(shader);
            renderer.Mesh.Draw(shader);
            shader.unBind();
        }
    }

private:
    Shader shader;
};

#endif //CG_ENGINE_MESHSYSTEM_H
