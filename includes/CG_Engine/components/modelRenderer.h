#ifndef CG_ENGINE_MODELRENDERER_H
#define CG_ENGINE_MODELRENDERER_H

#include <CG_Engine/source.h>

struct ModelRenderer : public BaseComponent {
    ~ModelRenderer() = default;

    ModelRenderer() : Name("PACK") {
        Renderer = Resource.getModel(Name);
    }

    ModelRenderer(std::string name) : Name(name) {
        Renderer = Resource.getModel(Name);
    }

    std::string Name;
    std::shared_ptr<Model> Renderer;
};

#endif //CG_ENGINE_MODELRENDERER_H
