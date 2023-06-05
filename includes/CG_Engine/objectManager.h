#ifndef CG_ENGINE_OBJECTMANAGER_H
#define CG_ENGINE_OBJECTMANAGER_H

#include <CG_Engine/objects/model.h>
#include <CG_Engine/render/light.h>

class Object {
private:
    Model model;
    Light *light;  // Pointer to a Light object

public:
    Object(const Model &objModel, Light *objLight)
            : model(objModel), light(objLight) {}

    void render(Shader &shader, int lightIndex) {
        model.Draw(shader);  // Render the model
        if (light != nullptr) {
            light->render(shader, lightIndex);  // Render the light if it exists
        }
    }
};

class Manager {
private:
    std::vector<Object> objects;
    int numLightsUsed;  // Number of lights used

public:
    Manager() : numLightsUsed(0) {}

    void addObject(const Object &obj) {
        if (numLightsUsed >= 30) {
            std::cout << "Maximum number of lights reached. Cannot add more objects with lights." << std::endl;
            return;
        }

        objects.push_back(obj);
        numLightsUsed++;
    }

    void renderObjects(Shader &shader) {
        for (int i = 0; i < objects.size(); i++) {
            objects[i].render(shader, i);
        }
    }
};

#endif //CG_ENGINE_OBJECTMANAGER_H
