#ifndef CG_ENGINE_MESH_H
#define CG_ENGINE_MESH_H

#include <string>
#include <utility>
#include <vector>

#include <CG_Engine/render/shader.h>
#include <CG_Engine/render/vArray.h>
#include <CG_Engine/render/material.h>

class GLMesh {
public:
    GLMesh() = default;

    ~GLMesh() = default;

    GLMesh(Vertex *vertices, GLsizei vSize) {
        vao = VertexArray(vertices, vSize);
    }

    GLMesh(Vertex *vertices, GLsizei vSize, GLuint *indices, GLsizei iSize) {
        vao = VertexArray(vertices, vSize, indices, iSize);
    }

    GLMesh(Vertex *vertices, GLsizei vSize, GLuint *indices, GLsizei iSize, const Material &matrial) :
            material(matrial) {
        vao = VertexArray(vertices, vSize, indices, iSize);
    }

    GLMesh(const VertexArray &vao) : vao(vao) {}

    void SetMaterial(const Material &matrial) {
        material = matrial;
    }

    Material &GetMaterial() {
        return material;
    }

    void render(Shader &shader, GLenum mode = GL_TRIANGLE_STRIP) {
        material.SetUniform(shader);
        shader.bind();
        vao.drawElements(mode);
    }

    void drawArrays(Shader &shader, GLenum mode = GL_TRIANGLES) {
        material.SetUniform(shader);
        shader.bind();
        vao.drawArrays(mode);
    }

private:
    VertexArray vao;
    Material material;
};

#endif //CG_ENGINE_MESH_H