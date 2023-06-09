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
    std::string name = "Object";
    unsigned int icon;

    Object(const Model &objModel, Light *objLight = nullptr)
            : model(objModel), light(objLight) {}

    Object(const Model &objModel, std::string name, Light *objLight = nullptr)
            : model(objModel), name(std::move(name)), light(objLight) {}

    Object(const Model &objModel, std::string name, unsigned int icon, Light *objLight = nullptr)
            : model(objModel), name(std::move(name)), light(objLight), icon(icon) {}

    void changeTexture(const std::string &texturePath, const char *type) {
        for (auto &mesh: model.meshes) {
            for (auto &texture: mesh.textures) {
                if (texture.type == type) {
                    unsigned int id = TextureFromFile(texturePath.c_str(), "");
                    if (id != FAIL_LOAD_TEXTURE)
                        texture.id = id;
                    return;
                }
            }
        }
    }

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
    std::vector<Object> base;
public:
    Manager() : numLightsUsed(0) {}

    Object getBaseByID(int id) const {
        return base[id];
    }

    Object &getByID(int id) {
        return objects[id];
    }

    void addObject(Object obj) {
        if (numLightsUsed >= 30) {
            std::cout << "Maximum number of lights reached. Cannot add more objects with lights." << std::endl;
            return;
        }

        objects.push_back(obj);
        numLightsUsed++;
    }

    void remove(int id) {
        if (id >= 0 && id < objects.size()) {
            objects.erase(objects.begin() + id);
            numLightsUsed--;
        }
    }

    void baseAddObject(const Object &obj) {
        base.push_back(obj);
    }

    void baseAddModel(const Model &model, const std::string name, unsigned int icon = 0) {
        base.emplace_back(Object(model, name, icon));
    }

    std::vector<unsigned int> getIcons() {
        std::vector<unsigned int> icons;
        for (const auto &obj: base) {
            icons.push_back(obj.icon);
        }
        return icons;
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

    unsigned long long last() const {
        return numLightsUsed;
    }

    unsigned long long size() const {
        return objects.size();
    }

    unsigned long long bSize() const {
        return base.size();
    }

    const char **getName() const {
        const char **charPtrArray = new const char *[size()];
        for (std::size_t i = 0; i < size(); i++) {
            charPtrArray[i] = objects[i].name.c_str();
        }

        return charPtrArray;
    }

    const char **getBaseName() const {
        const char **charPtrArray = new const char *[base.size()];
        for (std::size_t i = 0; i < base.size(); i++) {
            charPtrArray[i] = base[i].name.c_str();
        }

        return charPtrArray;
    }
};

#endif //CG_ENGINE_OBJECTMANAGER_H
