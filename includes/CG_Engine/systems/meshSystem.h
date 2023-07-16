#ifndef CG_ENGINE_MESHSYSTEM_H
#define CG_ENGINE_MESHSYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/meshRenderer.h>
#include <CG_Engine/engine.h>
#include <CG_Engine/ui/input.h>
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
        Mat4x4 model;
        for (auto entity: entities) {
            auto &renderer = Manager.getComponent<MeshRenderer>(entity);
            auto &transform = Manager.getComponent<Transform>(entity);

            shader.bind();
            model = Mat4x4(1.0f);
            model = model.translate(transform.Position);
            model = model.rotate(transform.Rotation.x, Vec3(1, 0, 0));
            model = model.rotate(transform.Rotation.y, Vec3(0, 1, 0));
            model = model.rotate(transform.Rotation.z, Vec3(0, 0, 1));
            model = model.scale(Vec3(transform.Scale));
            shader.setMat4("model", model);

            if (renderer.Name == "SPHERE")
                renderer.Mesh.render(shader);
            else {
                renderer.Mesh.drawArrays(shader);
            }
        }
    }

private:
    Shader shader;
};

#endif //CG_ENGINE_MESHSYSTEM_H
