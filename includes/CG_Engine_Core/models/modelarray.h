#ifndef CG_ENGINE_MODELARRAY_H
#define CG_ENGINE_MODELARRAY_H

#include <CG_Engine_Core/math/vec3.h>
#include <CG_Engine_Core/shader.h>
#include <vector>
#include "CG_Engine_Core/physics/rigidbody.h"

template<class T>
class ModelArray {
public:
    std::vector<RigidBody> instances;

    void init() {
        model.init();
    }

    void render(Shader &shader, float dt) {
        for (RigidBody &rb: instances) {
            rb.update(dt);
            model.rb.pos = rb.pos;
            model.render(shader, dt);
        }
    }

    void setSize(Vec3 size) {
        model.size = size;
    }

    void cleanup() {
        model.cleanup();
    }

protected:
    T model;
};

#endif //CG_ENGINE_MODELARRAY_H
