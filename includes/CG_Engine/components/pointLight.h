#ifndef CG_ENGINE_POINTLIGHT_H
#define CG_ENGINE_POINTLIGHT_H

#include <CG_Engine/base/baseComponent.h>
#include <CG_Engine/math/common.h>
#include <CG_Engine/render/light.h>

struct PointLighting : public BaseComponent {
    PointLighting() = default;

    ~PointLighting() = default;

    PointLight light = PointLight(Vec3(0.2f, 0.2f, 0.2f), 1.0f, 0.09f, 0.032f,
                                  Vec3(0.2f, 0.2f, 0.2f), Vec3(0.5f, 0.5f, 0.5f),
                                  Vec3(1.0f, 1.0f, 1.0f));
};

#endif //CG_ENGINE_POINTLIGHT_H
