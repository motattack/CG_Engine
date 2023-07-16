#ifndef CG_ENGINE_VARRAY_H
#define CG_ENGINE_VARRAY_H

#include <GL/glew.h>
#include <CG_Engine/render/vertex.h>

class VertexArray {
private:
    GLuint VAO, VBO, EBO;
    GLsizei numOfIndices, numOfVertices;
public:
    ~VertexArray() = default;

    VertexArray() : VAO(0), EBO(0), VBO(0), numOfIndices(0),
                    numOfVertices(0) {}

    VertexArray(Vertex *vertices, GLsizei vSize, GLuint *indices = nullptr, GLsizei iSize = 0) :
            numOfIndices(iSize), numOfVertices(vSize) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

        // Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);
        // Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, Normal));
        // Texcoords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
        numOfVertices /= sizeof(Vertex);
    }

    void pushAttrib(GLuint index, GLint size, GLsizei stride, GLvoid *ptr) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, size, ptr);
    }

    void bind() {
        glBindVertexArray(VAO);
    }

    void unBind() {
        glBindVertexArray(0);
    }

    void drawElements(GLenum mode = GL_TRIANGLE_STRIP) {
        glBindVertexArray(VAO);
        glDrawElements(mode, numOfIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void drawArrays(GLenum mode = GL_TRIANGLES) {
        glBindVertexArray(VAO);
        glDrawArrays(mode, 0, numOfVertices);
        glBindVertexArray(0);
    }
};

#endif //CG_ENGINE_VARRAY_H
