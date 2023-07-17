#ifndef CG_ENGINE_DIRECTIONALLIGHT_H
#define CG_ENGINE_DIRECTIONALLIGHT_H

#include <CG_Engine/render/light.h>

struct DirectionalLight : public BaseComponent {
    DirectionalLight() = default;

    ~DirectionalLight() = default;

    DirLight light = DirLight(Vec3(0.8f, 0.2f, 0.2f),
                              Vec3(0.4f, 0.4f, 0.4f),
                              Vec3(0.5f, 0.5f, 0.5f),
                              Vec3(-0.2f, -1.0f, -0.5f));
};

#endif //CG_ENGINE_DIRECTIONALLIGHT_H
