#ifndef CG_ENGINE_CORE_LAMP_H
#define CG_ENGINE_CORE_LAMP_H

// T

#include <CG_Engine_Core/light.h>
#include <CG_Engine_Core/models/box.h>
#include <CG_Engine_Core/models/cube.h>
#include <CG_Engine_Core/material.h>
#include <CG_Engine_Core/shader.h>

class Lamp : public Cube {
public:
    Vec3 lightColor;

    Lamp(unsigned int maxNoInstances, Vec3 lightColor = Vec3(1.0f))
            : Cube(maxNoInstances) {
        id = "lamp";
        this->lightColor = lightColor;
    }

    void render(Shader shader, float dt, Scene *scene, bool setModel = true) {
        // set light color
        shader.set3Float("lightColor", lightColor);

        Cube::render(shader, dt, scene, setModel);
    }
};


#endif //CG_ENGINE_CORE_LAMP_H
