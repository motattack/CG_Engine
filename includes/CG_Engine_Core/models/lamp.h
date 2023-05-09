#ifndef CG_ENGINE_CORE_LAMP_H
#define CG_ENGINE_CORE_LAMP_H

#include "cube.h"
#include "modelarray.h"
#include <CG_Engine_Core/light.h>

class Lamp : public Cube {
public:
    Vec3 lightColor;

    PointLight pointLight;

    explicit Lamp(Vec3 lightColor = Vec3(1.0f),
         Vec4 ambient = Vec4(1.0f),
         Vec4 diffuse = Vec4(1.0f),
         Vec4 specular = Vec4(1.0f),
         float k0 = 1.0f,
         float k1 = 0.07f,
         float k2 = 0.017f,
         Vec3 pos = Vec3(0.0f),
         Vec3 size = Vec3(1.0f))
            : lightColor(lightColor),
              pointLight({pos, k0, k1, k2, ambient, diffuse, specular}),
              Cube(pos, size) {}

    void render(Shader shader, float dt) {
        // set light color
        shader.set3Float("lightColor", lightColor);

        Cube::render(shader, dt);
    }
};

class LampArray : public ModelArray<Lamp> {
public:
    std::vector<PointLight> lightInstances;

    void init() {
        model = Lamp(Vec3(1.0f),
                     Vec4(0.05f, 0.05f, 0.05f, 1.0f), Vec4(0.8f, 0.8f, 0.8f, 1.0f), Vec4(1.0f),
                     1.0f, 0.07f, 0.032f,
                     Vec3(0.0f), Vec3(0.25f));
        model.init();
    }

    void render(Shader shader, float dt) {
        for (PointLight pl: lightInstances) {
            model.rb.pos = pl.position;

            model.render(shader, dt);
        }
    }
};

#endif //CG_ENGINE_CORE_LAMP_H
