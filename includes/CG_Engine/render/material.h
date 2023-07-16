#ifndef CG_ENGINE_MATERIAL_H
#define CG_ENGINE_MATERIAL_H

#include <GL/glew.h>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/math/common.h>

struct Material {
    Material() = default;

    ~Material() = default;

    Material(Vec3 ambient, Vec3 diffuse, Vec3 specular, float shininess) :
            ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
    float shininess;

    void SetUniform(Shader &shader) {
        shader.bind();
        shader.setV3Float("material.ambient", ambient);
        shader.setV3Float("material.diffuse", diffuse);
        shader.setV3Float("material.specular", specular);
        shader.setFloat("material.shininess", shininess);
    }
};

#endif //CG_ENGINE_MATERIAL_H
