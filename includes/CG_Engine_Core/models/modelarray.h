#ifndef CG_ENGINE_MODELARRAY_H
#define CG_ENGINE_MODELARRAY_H

#include <CG_Engine_Core/math/vec3.h>
#include <CG_Engine_Core/shader.h>
#include <vector>
#include "CG_Engine_Core/physics/rigidbody.h"
#include "box.h"

#define UPPER_BOUND 100

template<class T>
class ModelArray {
public:
    std::vector<RigidBody> instances;

    void init() {
        model.init();

        // generate positions VBO
        glGenBuffers(1, &posVBO);
        glBindBuffer(GL_ARRAY_BUFFER, posVBO);
        glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate size VBO
        glGenBuffers(1, &sizeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
        glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // set attribute pointers for each mesh
        for (unsigned int i = 0, size = model.meshes.size(); i < size; i++) {
            glBindVertexArray(model.meshes[i].VAO);
            // positions
            glBindBuffer(GL_ARRAY_BUFFER, posVBO);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(3);
            // sizes
            glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(4);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glVertexAttribDivisor(3, 1); // reset _3rd_ attribute every _1_ instance
            glVertexAttribDivisor(4, 1); // reset _4th_ attribute every _1_ instance

            glBindVertexArray(0);
        }
    }

    void render(Shader shader, float dt, Box *box, bool setLists = true) {
        if (setLists) {
            positions.clear();
            sizes.clear();

            for (RigidBody &rb: instances) {
                rb.update(dt);
                positions.push_back(rb.pos);
                sizes.push_back(model.size);
            }
        }

        shader.setMat4("model", Mat4x4(1.0f));

        model.render(shader, dt, nullptr, false, false);

        int instances = std::min(UPPER_BOUND, (int) positions.size()); // if more than 100 instances, only render 100

        // update data
        if (instances != 0) {
            // if instances exist

            glBindBuffer(GL_ARRAY_BUFFER, posVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, instances * 3 * sizeof(float), &positions[0]);

            glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, instances * 3 * sizeof(float), &sizes[0]);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        // render instanced data
        for (unsigned int i = 0, length = model.meshes.size(); i < length; i++) {
            for (unsigned int j = 0; j < instances; j++) {
                box->addInstance(model.meshes[i].br, positions[j], sizes[j]);
            }

            glBindVertexArray(model.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, model.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, instances);
            glBindVertexArray(0);
        }
    }

    void setSize(Vec3 size) {
        model.size = size;
    }

    void cleanup() {
        model.cleanup();
    }

protected:
    T model;

    unsigned int posVBO{};
    unsigned int sizeVBO{};
    std::vector<Vec3> positions;
    std::vector<Vec3> sizes;
};

#endif //CG_ENGINE_MODELARRAY_H