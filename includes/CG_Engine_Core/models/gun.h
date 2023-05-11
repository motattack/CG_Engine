#ifndef CG_ENGINE_GUN_H
#define CG_ENGINE_GUN_H

#include <CG_Engine_Core/model.h>
#include <CG_Engine_Core/UI/camera.h>
#include <CG_Engine_Core/physics/evn.h>
#include "../../../scene.h"

// T

class Gun : public Model {
public:
    Gun(unsigned int maxNoInstances)
            : Model("m4a1", BoundTypes::AABB, maxNoInstances, CONST_INSTANCES | NO_TEX) {}

    void init() {
        loadModel("res/assets/models/m4a1/scene.gltf");
    }

    void render(Shader shader, float dt, Scene *scene, bool setModel = false) {
        Mat4x4 model = Mat4x4(1.0f);

        // set position
        // multiply offset by unit vector in 2 directions
        instances[0]->pos =
                scene->getActiveCamera()->cameraPos + Vec3(scene->getActiveCamera()->cameraFront * 0.5f) -
                Vec3(scene->getActiveCamera()->cameraUp * 0.205f);
        model = model.translate(instances[0]->pos);

        float theta;

        // rotate around camera right using dot product
        theta = acos(Environment::worldUp.dotProduct(scene->getActiveCamera()->cameraFront) /
                     (scene->getActiveCamera()->cameraUp).len() /
                     (scene->getActiveCamera()->cameraFront).len());
        model = model.rotate(atanf(1) * 2 - theta,
                             scene->getActiveCamera()->cameraRight); // offset by pi/2 radians bc angle btwn camFront and gunFront

        // rotate around cameraUp using dot product
        Vec2 front2d = Vec2(scene->getActiveCamera()->cameraFront.x, scene->getActiveCamera()->cameraFront.z);
        theta = acos(Vec2(1.0f, 0.0f).dotProduct(front2d) / (front2d).len());
        model = model.rotate(scene->getActiveCamera()->cameraFront.z < 0 ? theta : -theta, Environment::worldUp);

        // scale
        model = model.Scale(instances[0]->size);

        shader.setMat4("model", model);

        Model::render(shader, dt, scene, false);
    }
};

#endif //CG_ENGINE_GUN_H
