#ifndef CG_ENGINE_VBUFFER_H
#define CG_ENGINE_VBUFFER_H

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

template<typename T>
class vBuffer {
private:
    unsigned int ID{};
public:
    vBuffer() = default;

    explicit vBuffer(T *data, GLsizeiptr size) {
        glGenBuffers(1, &ID);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    };

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    };

    void remove() {
        glDeleteBuffers(1, &ID);
    };
};

#endif //CG_ENGINE_VBUFFER_H
