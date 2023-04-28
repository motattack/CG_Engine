#ifndef CG_ENGINE_VBUFFER_H
#define CG_ENGINE_VBUFFER_H

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class vBuffer {
private:
    unsigned int ID{};
public:
    explicit vBuffer(float *data, GLsizeiptr size) {
        glGenBuffers(1, &ID);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        std::cout << data << std::endl;
    };

    void bind() const {
            glBindBuffer(GL_ARRAY_BUFFER, ID);
    };

    void remove() {
        glDeleteBuffers(1, &ID);
    };
};

#endif //CG_ENGINE_VBUFFER_H
