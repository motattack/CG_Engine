#ifndef CG_ENGINE_MESH_H
#define CG_ENGINE_MESH_H

#include <string>
#include <utility>
#include <vector>

#include <CG_Engine_Core/render/shader.h>
#include <CG_Engine_Core/render/texture.h>
#include <CG_Engine_Core/algo/bounds.h>
#include <CG_Engine_Core/models/box.h>

struct Vertex {
    Vec3 pos;
    Vec3 normal;
    Vec2 texCoord;

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
    ArrayObject VAO;

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

            VAO.bind();
            VAO.draw(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            ArrayObject::clear();

            // reset
            glActiveTexture(GL_TEXTURE0);
        }
    };

    void cleanup() {
        VAO.cleanup();
    };

private:
    unsigned int VBO{}, EBO{};

    bool noTex;

    void setup() {
        // create buffers/arrays

        // bind VAO
        VAO.generate();
        VAO.bind();

        // generate/set EBO
        VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
        VAO["EBO"].generate();
        VAO["EBO"].bind();
        VAO["EBO"].setData<GLuint>(indices.size(), &indices[0], GL_STATIC_DRAW);

        // generate/set VBO
        VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
        VAO["VBO"].generate();
        VAO["VBO"].bind();
        VAO["VBO"].setData<Vertex>(vertices.size(), &vertices[0], GL_STATIC_DRAW);

        // set vertex attrib pointers
        // vertex positions
        VAO["VBO"].setAttPointer<GLfloat>(0, 3, GL_FLOAT, 8, 0);
        // normal ray
        VAO["VBO"].setAttPointer<GLfloat>(1, 3, GL_FLOAT, 8, 3);
        // texture coordinates
        VAO["VBO"].setAttPointer<GLfloat>(2, 2, GL_FLOAT, 8, 6);

        VAO["VBO"].clear();

        ArrayObject::clear();
    };
};

#endif //CG_ENGINE_MESH_H