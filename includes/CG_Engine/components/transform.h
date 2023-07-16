#ifndef CG_ENGINE_TRANSFORM_H
#define CG_ENGINE_TRANSFORM_H

#include <CG_Engine/base/baseComponent.h>
#include <CG_Engine/math/common.h>
#include "CG_Engine/render/shader.h"

class Transform : public BaseComponent {
public:
    Transform() = default;

    ~Transform() = default;

    Vec3 Position = Vec3(0.0f);
    Vec3 Rotation = Vec3(0.0f);
    float Scale = 1.0f;

    Transform(const Vec3 &translate, const float scale, const float rotation) :
            Position(translate), Scale(scale), Rotation(rotation) {}

    Transform(const Vec3 &translate, const float scale) :
            Position(translate), Scale(scale) {}

    Transform(const Vec3 &translate) :
            Position(translate) {}

    Mat4x4 model() {
        auto model = Mat4x4(1.0f);
        model = model.translate(Position);
        model = model.rotate(radians(Rotation.x), Vec3(1, 0, 0));
        model = model.rotate(radians(Rotation.y), Vec3(0, 1, 0));
        model = model.rotate(radians(Rotation.z), Vec3(0, 0, 1));
        model = model.scale(Vec3(Scale));
        return model;
    }

    void setModelUniform(Shader &shader) {
        shader.bind();
        shader.setMat4("model", model());
    }
};

#endif //CG_ENGINE_TRANSFORM_H
