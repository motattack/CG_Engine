#ifndef CG_ENGINE_VBUFFER_H
#define CG_ENGINE_VBUFFER_H

#include <GL/glew.h>

class vBuffer {
private:
    unsigned int ID{};
public:
    explicit vBuffer(float *data) {
        glGenBuffers(1, &ID);
        bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    };

    void bind() const {
            glBindBuffer(GL_ARRAY_BUFFER, ID);
    };

    void remove() {
        glDeleteBuffers(1, &ID);
    };
};

#endif //CG_ENGINE_VBUFFER_H
