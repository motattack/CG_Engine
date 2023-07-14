#ifndef CG_ENGINE_TRANSFORM_H
#define CG_ENGINE_TRANSFORM_H

#include <CG_Engine/base/baseComponent.h>
#include <CG_Engine/math/common.h>

class Transform : public BaseComponent {
public:
    Transform() = default;

    ~Transform() = default;

    Vec3 Position = Vec3(0.0f);
    Vec3 Rotation = Vec3(0.0f);
    Vec3 Scale = Vec3(1.0f);

    Transform(const Vec3 &translate, const Vec3 &scale, const Vec3 &rotation) :
            Position(translate), Scale(scale), Rotation(rotation) {}

    Transform(const Vec3 &translate, const Vec3 &scale) :
            Position(translate), Scale(scale) {}

    Transform(const Vec3 &translate) :
            Position(translate) {}
};

#endif //CG_ENGINE_TRANSFORM_H
