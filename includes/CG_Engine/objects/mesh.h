#ifndef CG_ENGINE_MESH_H
#define CG_ENGINE_MESH_H

#include <string>
#include <utility>
#include <vector>

#include <CG_Engine/render/shader.h>
#include <CG_Engine/vertex/memory.h>

struct Vertex {
    Vec3 Position; // Позиция
    Vec3 Normal; // Нормаль
    Vec2 TexCoords; // Текстурные координаты
    Vec3 Tangent; // Касательный вектор
    Vec3 Bitangent; // Вектор бинормали (вектор, перпендикулярный касательному вектору и вектору нормали)
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

        EBO = veArray<unsigned int>(indices.data(), indices.size() * sizeof(unsigned int));

        // Координаты вершин
        vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr);

        // Нормали вершин
        vArray::attrPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));

        // Текстурные координаты вершин
        vArray::attrPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));

        // Касательный вектор вершины
        vArray::attrPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Tangent));

        // Вектор бинормали вершины
        vArray::attrPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Bitangent));

        glEnableVertexAttribArray(0);
    }

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh() = default;

    ~Mesh() = default;

    // Constructors
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = std::move(vertices);
        this->indices = std::move(indices);
        this->textures = std::move(textures);

        this->setupMesh();
    }

    Mesh(std::vector<Vertex> vertices) {
        this->vertices = std::move(vertices);

        this->setupMesh();
    }

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
        this->vertices = std::move(vertices);
        this->indices = std::move(indices);

        this->setupMesh();
    }

    void Draw(Shader &shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i); // перед связыванием активируем нужный текстурный юнит

            // Получаем номер текстуры (номер N в diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // конвертируем unsigned int в строку
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // конвертируем unsigned int в строку
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // конвертируем unsigned int в строку

            // Теперь устанавливаем сэмплер на нужный текстурный юнит
            glUniform1i(glGetUniformLocation(shader.program(), (name + number).c_str()), i);
            // и связываем текстуру
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glActiveTexture(GL_TEXTURE0);

        // Draw Mesh
        this->VAO.bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // возвращаем к первоначальным значениям
        glActiveTexture(GL_TEXTURE0);
    }
};

#endif //CG_ENGINE_MESH_H