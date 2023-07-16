#ifndef CG_ENGINE_VARRAY_LIB_H
#define CG_ENGINE_VARRAY_LIB_H

#include <CG_Engine/vertex/memory.h>

class vArray {
private:
    unsigned int ID{};
public:
    vArray() {
        glGenVertexArrays(1, &ID);
        bind();
    };

    static void
    attrPointer(GLuint ind, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
        glVertexAttribPointer(ind, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(ind);
    };

    void bind() const {
        glBindVertexArray(ID);
    };

    static void unbind() {
        glBindVertexArray(0);
    };

    void remove() {
        glDeleteVertexArrays(1, &ID);
    };
};

#endif //CG_ENGINE_VARRAY_LIB_H
