#ifndef CG_ENGINE_MATERIAL_H
#define CG_ENGINE_MATERIAL_H

#include <GL/glew.h>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/math/common.h>

struct Material {
    Material() = default;
    ~Material() = default;
    Material(Vec3 albedo, GLfloat metallic, GLfloat roughness, GLfloat ao):
            Albedo(albedo), Metallic(metallic), Roughness(roughness), Ao(ao) { }

    GLfloat Ao = 1.0f;
    GLfloat Metallic = 0.1f;
    GLfloat Roughness = 0.9f;
    Vec3 Albedo = Vec3(0.6f);

    GLuint AoMap = 0;
    GLuint NormalMap = 0;
    GLuint AlbedoMap = 0;
    GLuint MetallicMap = 0;
    GLuint RoughnessMap = 0;

    void SetUniform(Shader& shader) {
        shader.bind();
        shader.setFloat("material.Ao", Ao);
        shader.setV3Float("material.Albedo", Albedo);
        shader.setFloat("material.Metallic", Metallic);
        shader.setFloat("material.Roughness", Roughness);

        shader.SetSample2D("AoMap", AoMap, 0);
        shader.SetSample2D("AlbedoMap", AlbedoMap, 1);
        shader.SetSample2D("NormalMap", NormalMap, 2);
        shader.SetSample2D("MetallicMap", MetallicMap, 3);
        shader.SetSample2D("RoughnessMap", RoughnessMap, 4);
    }
};

#endif //CG_ENGINE_MATERIAL_H
