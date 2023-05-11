#ifndef CG_ENGINE_SPHERE_H
#define CG_ENGINE_SPHERE_H
// T
#include <CG_Engine_Core/model.h>

class Sphere : public Model {
public:
    Sphere(unsigned int maxNoInstances)
            : Model("sphere", BoundTypes::SPHERE, maxNoInstances, NO_TEX | DYNAMIC) {}

    void init() {
        loadModel("res/assets/models/sphere/scene.gltf");
    }
};

#endif //CG_ENGINE_SPHERE_H
