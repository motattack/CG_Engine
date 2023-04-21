#ifndef CG_ENGINE_VBUFFER_H
#define CG_ENGINE_VBUFFER_H

#include <GL/glew.h>

class vBuffer {
private:
    unsigned int ID{};
public:
    vBuffer(float *data, int size) {
        glGenBuffers(1, &ID);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(data), data, GL_STATIC_DRAW);
    };

    void bind(int f = -1) const {
        if (!f)
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        else
            glBindBuffer(GL_ARRAY_BUFFER, ID);
    };

    void remove() {
        glDeleteBuffers(1, &ID);
    };
};

#endif //CG_ENGINE_VBUFFER_H
