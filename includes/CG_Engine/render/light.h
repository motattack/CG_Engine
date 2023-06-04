#ifndef CG_ENGINE_LIGHT_H
#define CG_ENGINE_LIGHT_H

#include <CG_Engine/render/shader.h>

struct DirLight {
    Vec3 direction;

    Vec4 ambient;
    Vec4 diffuse;
    Vec4 specular;

    void render(Shader shader) const {
        shader.set3Float("dirLight.direction", direction);
        shader.set4Float("dirLight.ambient", ambient);
        shader.set4Float("dirLight.diffuse", diffuse);
        shader.set4Float("dirLight.specular", specular);
    }
};

struct PointLight {
    Vec3 position;

    // attenuation constants
    float k0;
    float k1;
    float k2;

    Vec4 ambient;
    Vec4 diffuse;
    Vec4 specular;

    void render(Shader shader, int idx) const {
        std::string name = "pointLights[" + std::to_string(idx) + "]";
        shader.set3Float(name + ".position", position);

        shader.setFloat(name + ".k0", k0);
        shader.setFloat(name + ".k1", k1);
        shader.setFloat(name + ".k2", k2);

        shader.set4Float(name + ".ambient", ambient);
        shader.set4Float(name + ".diffuse", diffuse);
        shader.set4Float(name + ".specular", specular);
    }
};

struct SpotLight {
    Vec3 position;
    Vec3 direction;

    float cutOff;
    float outerCutOff;

    // attenuation constants
    float k0;
    float k1;
    float k2;

    Vec4 ambient;
    Vec4 diffuse;
    Vec4 specular;

    void render(Shader shader, int idx) const {
        std::string name = "spotLights[" + std::to_string(idx) + "]";
        shader.set3Float(name + ".position", position);
        shader.set3Float(name + ".direction", direction);

        shader.setFloat(name + ".cutOff", cutOff);
        shader.setFloat(name + ".outerCutOff", outerCutOff);

        shader.setFloat(name + ".k0", k0);
        shader.setFloat(name + ".k1", k1);
        shader.setFloat(name + ".k2", k2);

        shader.set4Float(name + ".ambient", ambient);
        shader.set4Float(name + ".diffuse", diffuse);
        shader.set4Float(name + ".specular", specular);
    }
};


#endif //CG_ENGINE_LIGHT_H
