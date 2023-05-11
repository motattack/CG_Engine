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

// T

struct Vertex {
    /*
        vertex values
    */
    // position
    Vec3 pos;
    // normal vector
    Vec3 normal;
    // texture coordinate
    Vec2 texCoord;

    // generate list of vertices
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

/*
    class representing Mesh
*/

class Mesh {
public:
    // Bounding region for mesh
    BoundingRegion br;

    // list of vertices
    std::vector<Vertex> vertices;
    // list of indices
    std::vector<unsigned int> indices;
    // vertex array object pointing to all data for the mesh
    ArrayObject VAO;

    // texture list
    std::vector<Texture> textures;
    // material diffuse value
    aiColor4D diffuse;
    // material specular value
    aiColor4D specular;

    /*
        constructors
    */

    // default
    Mesh() {};

    // initialize as textured object
    Mesh(BoundingRegion br, std::vector<Texture> textures = {}) : br(br), textures(textures), noTex(false) {};

    // initialize as material object
    Mesh(BoundingRegion br, aiColor4D diff, aiColor4D spec) : br(br), diffuse(diff), specular(spec), noTex(true) {};

    // load vertex and index data
    void loadData(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices) {
        this->vertices = _vertices;
        this->indices = _indices;

        // bind VAO
        VAO.generate();
        VAO.bind();

        // generate/set EBO
        VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
        VAO["EBO"].generate();
        VAO["EBO"].bind();
        VAO["EBO"].setData<GLuint>(this->indices.size(), &this->indices[0], GL_STATIC_DRAW);

        // load data into vertex buffers
        VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
        VAO["VBO"].generate();
        VAO["VBO"].bind();
        VAO["VBO"].setData<Vertex>(this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        VAO["VBO"].bind();
        // vertex Positions
        VAO["VBO"].setAttPointer<GLfloat>(0, 3, GL_FLOAT, 8, 0);
        // normal ray
        VAO["VBO"].setAttPointer<GLfloat>(1, 3, GL_FLOAT, 8, 3);
        // vertex texture coords
        VAO["VBO"].setAttPointer<GLfloat>(2, 3, GL_FLOAT, 8, 6);

        VAO["VBO"].clear();

        ArrayObject::clear();
    };

    // render number of instances using shader
    void render(Shader shader, unsigned int noInstances) {
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

        VAO.bind();
        VAO.draw(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, noInstances);
        ArrayObject::clear();

        glActiveTexture(GL_TEXTURE0);
    };

    // free up memory
    void cleanup() {
        VAO.cleanup();
    };

private:
    // true if has only materials
    bool noTex;

    // setup data with buffers
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
