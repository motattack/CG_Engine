#ifndef CG_ENGINE_MESH_H
#define CG_ENGINE_MESH_H

#include <string>
#include <utility>
#include <vector>

#include <CG_Engine_Core/shader.h>
#include <CG_Engine_Core/vertex/vbuffer.h>
#include <CG_Engine_Core/vertex/varray.h>
#include <CG_Engine_Core/vertex/vearray.h>
#include "texture.h"
#include "CG_Engine_Core/algo/bounds.h"
#include "CG_Engine_Core/models/box.h"

struct Vertex {
    Vec3 pos;
    Vec3 normal;
    Vec2 texCoord;

    /*
    v1.vec3.x|v1.vec3.y|v1.vec3.z|v1.vec2.x|v1.vec2.y|
    v2.vec3.x|v2.vec3.y|v2.vec3.z|v2.vec2.x|v2.vec2.y
    */

    static std::vector<Vertex> genList(float *vertices, int noVertices) {
        std::vector<Vertex> ret(noVertices);

        int stride = sizeof(Vertex) / sizeof(float);

        for (int i = 0; i < noVertices; i++) {
            ret[i].pos = Vec3(
                    vertices[i * stride + 0],
                    vertices[i * stride + 1],
                    vertices[i * stride + 2]
            );

            ret[i].normal = Vec3(
                    vertices[i * stride + 3],
                    vertices[i * stride + 4],
                    vertices[i * stride + 5]
            );

            ret[i].texCoord = Vec2(
                    vertices[i * stride + 6],
                    vertices[i * stride + 7]
            );
        }

        return ret;
    };
};

class Mesh {
public:
    BoundingRegion br;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO{};

    std::vector<Texture> textures;
    aiColor4D diffuse;
    aiColor4D specular;

    Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Texture> textures = {}) : br(br), vertices(std::move(vertices)), indices(std::move(indices)),
                                               textures(std::move(textures)),
                                               noTex(false) {
        setup();
    };

    Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse,
         aiColor4D specular) : br(br), vertices(std::move(vertices)), indices(std::move(indices)), diffuse(diffuse),
                               specular(specular),
                               noTex(true) {
        setup();
    };

    void render(Shader shader, Vec3 pos, Vec3 size, Box *box, bool doRender = true) {
        if (noTex) {
            // materials
            shader.set4Float("material.diffuse", diffuse);
            shader.set4Float("material.specular", specular);
            shader.setInt("noTex", 1);
        } else {
            // textures
            unsigned int diffuseIdx = 0;
            unsigned int specularIdx = 0;

            for (unsigned int i = 0; i < textures.size(); i++) {
                // activate texture
                glActiveTexture(GL_TEXTURE0 + i);

                // retrieve texture info
                std::string name;
                switch (textures[i].type) {
                    case aiTextureType_DIFFUSE:
                        name = "diffuse" + std::to_string(diffuseIdx++);
                        break;
                    case aiTextureType_SPECULAR:
                        name = "specular" + std::to_string(specularIdx++);
                        break;
                }

                // set the shader value
                shader.setInt(name, i);
                // bind texture
                textures[i].bind();
            }
        }

        if (doRender) {
            box->addInstance(br, pos, size);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // reset
            glActiveTexture(GL_TEXTURE0);
        }
    };

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    };

private:
    unsigned int VBO{}, EBO{};

    bool noTex;

    void setup() {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO); // vertex array object
        glGenBuffers(1, &VBO); // vertex buffer object
        glGenBuffers(1, &EBO); // element buffer object

        glBindVertexArray(VAO);

        // load data into VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // load data into EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set vertex attribute pointers
        // vertex.position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
        // vertex.normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (offsetof(Vertex, normal)));
        // vertex.texCoord
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (offsetof(Vertex, texCoord)));

        glBindVertexArray(0);
    };
};

#endif //CG_ENGINE_MESH_H
