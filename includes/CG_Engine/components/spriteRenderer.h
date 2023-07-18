#ifndef CG_ENGINE_SPRITERENDERER_H
#define CG_ENGINE_SPRITERENDERER_H

#include <CG_Engine/base/baseComponent.h>
#include <CG_Engine/math/common.h>
#include <CG_Engine/render/shader.h>

struct SpriteRenderer : public BaseComponent {
    ~SpriteRenderer() = default;

    SpriteRenderer() = default;

    SpriteRenderer(std::string name) : TexName(name) {
        TexID = Resource.Texture2D(TexName);
    }

    void SetUniform(Shader &shader) {
        shader.bind();
        shader.SetSample2D("Sprite", TexID);
    }

    GLuint TexID = 0;
    std::string TexName;
};

#endif //CG_ENGINE_SPRITERENDERER_H
