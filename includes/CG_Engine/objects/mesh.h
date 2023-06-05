#ifndef CG_ENGINE_MESH_H
#define CG_ENGINE_MESH_H

#include <string>
#include <vector>

#include <CG_Engine/render/shader.h>

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
    unsigned int VAO, VBO, EBO;

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), &index[0], GL_STATIC_DRAW);

        // Position Attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

        // Normals Attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));

        // Texture Attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(0);
    }

public:
    // Mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> index;
    std::vector<Texture> textures;

    // Constructors
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> index, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->index = index;
        this->textures = textures;

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

            shader.setFloat(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glActiveTexture(GL_TEXTURE0);

        // Draw Mesh
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

#endif //CG_ENGINE_MESH_H