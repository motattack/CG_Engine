#ifndef CG_ENGINE_VEARRAY_H
#define CG_ENGINE_VEARRAY_H

#include <CG_Engine/vertex/memory.h>

template<typename T>
class veArray {
private:
    unsigned int ID{};
public:
    veArray() = default;

    explicit veArray(T *data, GLsizeiptr size) {
        glGenBuffers(1, &ID);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    };

    void bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    };

    void remove() {
        glDeleteBuffers(1, &ID);
    };
};

#endif //CG_ENGINE_VEARRAY_H
