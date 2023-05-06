#ifndef CG_ENGINE_MESH_H
#define CG_ENGINE_MESH_H

#include <string>
#include <utility>
#include <vector>

#include <CG_Engine_Core/shader.h>
#include <CG_Engine_Core/vertex/vbuffer.h>
#include <CG_Engine_Core/vertex/varray.h>
#include <CG_Engine_Core/vertex/vearray.h>

struct Vertex {
    Vec3 Position;
    Vec3 Normal;
    Vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
private:
    veArray<unsigned int> EBO;
    vArray VAO;
    vBuffer<Vertex> VBO;

    void setupMesh() {
        VBO = vBuffer<Vertex>(&vertices[0], vertices.size() * sizeof(Vertex));
        EBO = veArray(&index[0], index.size() * sizeof(unsigned int));

        // Position Attribute
        vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

        // Normals Attribute
        vArray::attrPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));

        // Texture Attribute
        vArray::attrPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(0);
    }

public:
    // Mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> index;
    std::vector<Texture> textures;

    // Constructors
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> index, std::vector<Texture> textures) {
        this->vertices = std::move(vertices);
        this->index = std::move(index);
        this->textures = std::move(textures);

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

            shader.setFloat("material." + name + number, i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glActiveTexture(GL_TEXTURE0);

        // Draw Mesh
        this->VAO.bind();
        glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

#endif //CG_ENGINE_MESH_H
