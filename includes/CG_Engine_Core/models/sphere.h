#ifndef CG_ENGINE_SPHERE_H
#define CG_ENGINE_SPHERE_H

#include <CG_Engine_Core/model.h>
#include <CG_Engine_Core/models/modelarray.h>


class Sphere : public Model {
public:
    Sphere(Vec3 pos = Vec3(0.0f), Vec3 size = Vec3(1.0f))
            : Model(BoundTypes::SPHERE, pos, size, true) {
        rb.mass = 1.0f;
    }

    void init() {
        loadModel("res/assets/models/sphere/scene.gltf");
    }
};

class SphereArray : public ModelArray<Sphere> {
public:
    void init() {
        model = Sphere(Vec3(0.0f), Vec3(0.25f));
        ModelArray::init();
    }
};

#endif //CG_ENGINE_SPHERE_H
