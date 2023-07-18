#ifndef CG_ENGINE_QUAD_H
#define CG_ENGINE_QUAD_H

#include <CG_Engine/vertex/varray.h>

class GLQuad {
public:
    ~GLQuad() = default;

    GLQuad() {
        vArray vertices[] = {
                Vertex(glm::vec3(-0.5f,  -0.5f, 0.0f), glm::vec3(0), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3( 0.5f,  -0.5f, 0.0f), glm::vec3(0), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3( 0.5f,   0.5f, 0.0f), glm::vec3(0), glm::vec2(1.0f, 1.0f)),
                Vertex(glm::vec3(-0.5f,   0.5f, 0.0f), glm::vec3(0), glm::vec2(0.0f, 1.0f))
        };

        GLuint indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        VAO = vArray(vertices, 4, indices, 6);
    }

    vArray VAO;
};

#endif //CG_ENGINE_QUAD_H
