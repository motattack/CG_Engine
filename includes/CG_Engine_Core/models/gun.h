#ifndef CG_ENGINE_GUN_H
#define CG_ENGINE_GUN_H

#include "../model.h"
#include "../camera.h"

class Gun : public Model {
public:
    Gun()
            : Model(Vec3(0.0f), Vec3(0.05f), true) {}

    void render(Shader shader, float dt, bool setModel = false) {
        auto model = Mat4x4(1.0f);

        // set position
        // multiply offset by unit vector in 2 directions
        rb.pos = Camera::defaultCamera.cameraPos + Vec3(Camera::defaultCamera.cameraFront * 0.5f) -
                 Vec3(Camera::defaultCamera.cameraUp * 0.205f);;
        model = model.translate(rb.pos);

        float theta;

        // rotate around camera right using dot product
        theta = acos(Camera::defaultCamera.worldUp.dotProduct(Camera::defaultCamera.cameraFront) /
                     Camera::defaultCamera.cameraUp.len() / Camera::defaultCamera.cameraFront.len());
        model = model.rotate(atanf(1) * 2 - theta,
                             Camera::defaultCamera.cameraRight); // offset by pi/2 radians bc angle btwn camFront and gunFront

        // rotate around cameraUp using dot product
        Vec2 front2d = Vec2(Camera::defaultCamera.cameraFront.x, Camera::defaultCamera.cameraFront.z);
        theta = acos(Vec2(1.0f, 0.0f).dotProduct(front2d) / front2d.len());

        model = model.rotate(Camera::defaultCamera.cameraFront.z < 0 ? theta : -theta,
                             Camera::defaultCamera.worldUp);

        // scale
        model = model.Scale(size);

        shader.setMat4("model", model);

        Model::render(shader, dt, false);
    }
};

#endif //CG_ENGINE_GUN_H
