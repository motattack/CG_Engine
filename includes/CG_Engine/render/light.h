#ifndef CG_ENGINE_LIGHT_H
#define CG_ENGINE_LIGHT_H

#include <CG_Engine/render/shader.h>

class Light {
protected:
    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;

public:
    Light(const Vec3 &ambientColor, const Vec3 &diffuseColor, const Vec3 &specularColor)
            : ambient(ambientColor), diffuse(diffuseColor), specular(specularColor) {}

    virtual void render(Shader &shader, int idx) const = 0;
};

struct DirLight : public Light {
    Vec3 direction;

    DirLight(const Vec3 &lightDirection, const Vec3 &ambientColor, const Vec3 &diffuseColor, const Vec3 &specularColor)
            : Light(ambientColor, diffuseColor, specularColor), direction(lightDirection) {}

    void render(Shader &shader, int /*idx*/) const override {
        shader.setV3Float("dirLight.direction", direction);
        shader.setV3Float("dirLight.ambient", ambient);
        shader.setV3Float("dirLight.diffuse", diffuse);
        shader.setV3Float("dirLight.specular", specular);
    }
};

struct PointLight : public Light {
    Vec3 position;

    // attenuation constants
    float constant;
    float linear;
    float quadratic;

    PointLight(const Vec3 &lightPosition, float attConstant, float attLinear, float attQuadratic,
               const Vec3 &ambientColor, const Vec3 &diffuseColor, const Vec3 &specularColor)
            : Light(ambientColor, diffuseColor, specularColor), position(lightPosition),
              constant(attConstant), linear(attLinear), quadratic(attQuadratic) {}

    void render(Shader &shader, int idx) const override {
        std::string name = "pointLights[" + std::to_string(idx) + "]";
        shader.setV3Float(name + ".position", position);
        shader.setV3Float(name + ".ambient", ambient);
        shader.setV3Float(name + ".diffuse", diffuse);
        shader.setV3Float(name + ".specular", specular);
        shader.setFloat(name + ".constant", constant);
        shader.setFloat(name + ".linear", linear);
        shader.setFloat(name + ".quadratic", quadratic);
    }
};

struct SpotLight : public Light {
    Vec3 position;
    Vec3 direction;

    Vec3 viewPos;

    float cutOff;
    float outerCutOff;

    // attenuation constants
    float constant;
    float linear;
    float quadratic;

    SpotLight(const Vec3 &lightPosition, const Vec3 &lightDirection, float innerAngle, float outerAngle,
              float attConstant, float attLinear, float attQuadratic,
              const Vec3 &ambientColor, const Vec3 &diffuseColor, const Vec3 &specularColor, const Vec3 &viewPos)
            : Light(ambientColor, diffuseColor, specularColor), position(lightPosition), direction(lightDirection),
              cutOff(std::cos(radians(innerAngle))), outerCutOff(std::cos(radians(outerAngle))),
              constant(attConstant), linear(attLinear), quadratic(attQuadratic), viewPos(viewPos) {}

    void render(Shader &shader, int idx) const override {
        std::string name = "spotLight";
        shader.setV3Float(name + ".position", position);
        shader.setV3Float(name + ".ambient", ambient);
        shader.setV3Float(name + ".diffuse", diffuse);
        shader.setV3Float(name + ".specular", specular);
        shader.setV3Float(name + ".direction", direction);

        shader.setV3Float("viewPos", viewPos);

        shader.setFloat(name + ".constant", constant);
        shader.setFloat(name + ".linear", linear);
        shader.setFloat(name + ".quadratic", quadratic);

        shader.setFloat(name + ".cutOff", cutOff);
        shader.setFloat(name + ".outerCutOff", outerCutOff);
    }
};

#endif //CG_ENGINE_LIGHT_H
