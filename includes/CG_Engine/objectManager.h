#ifndef CG_ENGINE_OBJECTMANAGER_H
#define CG_ENGINE_OBJECTMANAGER_H

#include <CG_Engine/objects/model.h>
#include <CG_Engine/render/light.h>

class Object {
private:
    Model model;
    Light *light;  // Pointer to a Light object

public:
    Vec3 pos;
    Vec3 rotation;
    float scale = 1.f;

    Object(const Model &objModel, Light *objLight = nullptr)
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

    Object &getByID(int id) {
        return objects[id];
    }

    void addObject(const Object &obj) {
        if (numLightsUsed >= 30) {
            std::cout << "Maximum number of lights reached. Cannot add more objects with lights." << std::endl;
            return;
        }

        objects.push_back(obj);
        numLightsUsed++;
    }

    void renderObjects(Shader &shader) {
        Mat4x4 model;
        for (int i = 0; i < objects.size(); i++) {
            model = Mat4x4(1.0f);
            model = model.translate(objects[i].pos);
            model = model.rotate(objects[i].rotation.x, Vec3(1, 0, 0));
            model = model.rotate(objects[i].rotation.y, Vec3(0, 1, 0));
            model = model.rotate(objects[i].rotation.z, Vec3(0, 0, 1));
            model = model.scale(Vec3(objects[i].scale));
            shader.setMat4("model", model);
            objects[i].render(shader, i);
        }
    }

    unsigned long long size() const {
        return objects.size();
    }

    const char **getName() const {
        std::vector<std::string> names;
        for (int i = 0; i < size(); i++) {
            names.emplace_back("Object" + std::to_string(i));
        }

        const char **charPtrArray = new const char *[names.size()];
        for (std::size_t i = 0; i < names.size(); i++) {
            charPtrArray[i] = names[i].c_str();
        }

        return charPtrArray;
    }
};

#endif //CG_ENGINE_OBJECTMANAGER_H
