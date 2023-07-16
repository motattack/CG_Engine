#ifndef CG_ENGINE_MESHRENDERER_H
#define CG_ENGINE_MESHRENDERER_H

#include <CG_Engine/base/baseComponent.h>
#include <string>
#include <CG_Engine/source.h>

struct MeshRenderer : public BaseComponent {
    ~MeshRenderer() = default;

    MeshRenderer(std::string name = "CUBE") : Name(name) {
        Mesh = Resource.getMesh(Name);
    }

    Mesh Mesh;
    std::string Name;
};

#endif //CG_ENGINE_MESHRENDERER_H
