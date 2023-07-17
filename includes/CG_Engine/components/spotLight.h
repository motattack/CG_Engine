#ifndef CG_ENGINE_SPOTLIGHT_H
#define CG_ENGINE_SPOTLIGHT_H

#include <CG_Engine/base/baseComponent.h>
#include <CG_Engine/math/common.h>
#include <CG_Engine/render/light.h>

struct SpotLighting : public BaseComponent {
    SpotLighting() = default;

    ~SpotLighting() = default;

    SpotLight light = SpotLight(Vec3(0.2f, 0.2f, 0.2f),
                                Vec3(0.2f, 0.2f, 0.2f),
                                0.0f, 0.0f,
                                1.0f,
                                0.09f,
                                0.032f,
                                Vec3(0.2f, 0.2f, 0.2f),
                                Vec3(1.0f, 1.0f, 1.0f),
                                Vec3(1.0f, 1.0f, 1.0f),
                                Vec3(1.0f, 1.0f, 1.0f)
    );
};

#endif //CG_ENGINE_SPOTLIGHT_H
