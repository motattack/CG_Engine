#ifndef CG_ENGINE_CORE_LIGHT_H
#define CG_ENGINE_CORE_LIGHT_H

#include <CG_Engine_Core/render/shader.h>

// T

struct DirLight {
    // direction (constant for all parallel rays)
    Vec3 direction;

    // light values
    Vec4 ambient;
    Vec4 diffuse;
    Vec4 specular;

    // render light into shader
    void render(Shader shader) {
        // set direction
        shader.set3Float("dirLight.direction", direction);

        // set lighting values
        shader.set4Float("dirLight.ambient", ambient);
        shader.set4Float("dirLight.diffuse", diffuse);
        shader.set4Float("dirLight.specular", specular);
    };
};

/*
    point light (eg light bulb)
*/

struct PointLight {
    // position
    Vec3 position;

    // attenuation constants
    float k0;
    float k1;
    float k2;

    // light values
    Vec4 ambient;
    Vec4 diffuse;
    Vec4 specular;

    // render light into shader
    void render(Shader shader, int idx) {
        // get name with index in array
        std::string name = "pointLights[" + std::to_string(idx) + "]";

        // set position
        shader.set3Float(name + ".position", position);

        // set attenuation constants
        shader.setFloat(name + ".k0", k0);
        shader.setFloat(name + ".k1", k1);
        shader.setFloat(name + ".k2", k2);

        // set lighting values
        shader.set4Float(name + ".ambient", ambient);
        shader.set4Float(name + ".diffuse", diffuse);
        shader.set4Float(name + ".specular", specular);
    };
};

/*
    spot light (flashlight)
*/

struct SpotLight {
    // position
    Vec3 position;
    // direction
    Vec3 direction;

    // first level cut off
    float cutOff;
    // second level cut off
    float outerCutOff;

    // attenuation constants
    float k0;
    float k1;
    float k2;

    // light values
    Vec4 ambient;
    Vec4 diffuse;
    Vec4 specular;

    // render light into shader
    void render(Shader shader, int idx) {
        // get name with index in array
        std::string name = "spotLights[" + std::to_string(idx) + "]";

        // set position
        shader.set3Float(name + ".position", position);
        // set direction
        shader.set3Float(name + ".direction", direction);

        // set first level cut off
        shader.setFloat(name + ".cutOff", cutOff);
        // set second level cut off
        shader.setFloat(name + ".outerCutOff", outerCutOff);

        // set attenuation constants
        shader.setFloat(name + ".k0", k0);
        shader.setFloat(name + ".k1", k1);
        shader.setFloat(name + ".k2", k2);

        // set lighting values
        shader.set4Float(name + ".ambient", ambient);
        shader.set4Float(name + ".diffuse", diffuse);
        shader.set4Float(name + ".specular", specular);
    };
};


#endif //CG_ENGINE_CORE_LIGHT_H
