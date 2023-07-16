#ifndef CG_ENGINE_MESH_H
#define CG_ENGINE_MESH_H

#include <string>
#include <utility>
#include <vector>

#include <CG_Engine/render/shader.h>
#include <CG_Engine/vertex/memory.h>

struct Vertex {
    Vec3 Position;
    Vec3 Normal;
    Vec2 TexCoords;

    static std::vector<Vertex> genList(float *vertices, int noVertices) {
        std::vector<Vertex> ret(noVertices);

        int stride = sizeof(Vertex) / sizeof(float);

        for (int i = 0; i < noVertices; i++) {
            ret[i].Position = Vec3(
                    vertices[i * stride + 0],
                    vertices[i * stride + 1],
                    vertices[i * stride + 2]
            );

            ret[i].Normal = Vec3(
                    vertices[i * stride + 3],
                    vertices[i * stride + 4],
                    vertices[i * stride + 5]
            );

            ret[i].TexCoords = Vec2(
                    vertices[i * stride + 6],
                    vertices[i * stride + 7]
            );
        }

        return ret;
    };
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
private:
    vArray VAO;
    vBuffer<Vertex> VBO;
    veArray<unsigned int> EBO;

    void setupMesh() {
        VBO = vBuffer<Vertex>(vertices.data(), vertices.size() * sizeof(Vertex));

        EBO = veArray<unsigned int>(index.data(), index.size() * sizeof(unsigned int));

        // Position Attribute
        vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr);

        // Normals Attribute
        vArray::attrPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));

        // Texture Attribute
        vArray::attrPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(0);
    }

public:
    // Mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> index;
    std::vector<Texture> textures;

    Mesh() = default;

    ~Mesh() = default;

    // Constructors
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> index, std::vector<Texture> textures) {
        this->vertices = std::move(vertices);
        this->index = std::move(index);
        this->textures = std::move(textures);

        this->setupMesh();
    }

    Mesh(std::vector<Vertex> vertices) {
        this->vertices = std::move(vertices);

        this->setupMesh();
    }

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> index) {
        this->vertices = std::move(vertices);
        this->index = std::move(index);

        this->setupMesh();
    }

    void Draw(Shader &shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string name = textures[i].type;
            std::string number;

            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.setFloat("material." + name + number, static_cast<float>(i));
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glActiveTexture(GL_TEXTURE0);

        // Draw Mesh
        this->VAO.bind();
        glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
};

#endif //CG_ENGINE_MESH_H