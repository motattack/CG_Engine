#ifndef CG_ENGINE_SCENE_H
#define CG_ENGINE_SCENE_H

#include <vector>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <CG_Engine_Core/shader.h>
#include <CG_Engine_Core/UI/camera.h>
#include <CG_Engine_Core/math/vec3.h>
#include <CG_Engine_Core/light.h>
#include <CG_Engine_Core/algo/states.h>
#include <CG_Engine_Core/algo/octree.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include "CG_Engine_Core/UI/mouse.h"
#include "CG_Engine_Core/UI/keyboard.h"

// forward declarations
namespace Octree {
    class node;
}

class Model;

class Scene {
public:
    // tries to store models/instances
    trie::Trie<Model *> models;
    trie::Trie<RigidBody *> instances;

    // list of instances that should be deleted
    std::vector<RigidBody *> instancesToDelete;

    // pointer to root node in octree
    Octree::node *octree;

    /*
        callbacks
    */
    // window resize
    void framebufferSizeCallback(int width, int height) {
        glViewport(0, 0, width, height);
        scrWidth = width;
        scrHeight = height;
    };

    /*
        constructor
    */
    Scene() : currentId("aaaaaaaa") {}

    Scene(const char *title, unsigned int scrWidth, unsigned int scrHeight) : title(title),
                                                                              activeCamera(-1),
                                                                              activePointLights(0), activeSpotLights(0),
                                                                              currentId("aaaaaaaa") {


        this->scrWidth = scrWidth;
        this->scrHeight = scrHeight;

        setWindowColor(0.1f, 0.15f, 0.15f, 1.0f);
    };

    /*
        initialization
    */
    bool init() {
        window.setFramerateLimit(75);
        window.setVerticalSyncEnabled(true);
        window.setActive(true);

        ImGui::SFML::Init(window, true);


        // Enable docking
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Set context settings properties
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.majorVersion = 4;
        settings.minorVersion = 6;
        settings.attributeFlags = sf::ContextSettings::Default;

        window.create(sf::VideoMode(scrWidth, scrHeight), "First Window", sf::Style::Titlebar | sf::Style::Close,
                      settings);

        glewExperimental = GL_TRUE;

        if (GLEW_OK != glewInit()) {
            std::cout << "Error: glew not init =(" << std::endl;
            throw;
        }

        glEnable(GL_DEPTH_TEST);

        octree = new Octree::node(BoundingRegion(Vec3(-16.0f), Vec3(16.0f)));

        return true;
    };

    void prepare(Box &box) {
        octree->update(box);
    };

    /*
        main loop methods
    */
    // process input
    void processInput(float dt) {
        if (activeCamera != -1 && activeCamera < cameras.size()) {

            // set camera direction
            double dx = Mouse::getDX(), dy = Mouse::getDY();
            if (dx != 0 || dy != 0) {
                cameras[activeCamera]->updateCameraDirection(dx, dy);
            }

            // set camera zoom
            double scrollDy = Mouse::getScrollDY();
            if (scrollDy != 0) {
                cameras[activeCamera]->updateCameraZoom(scrollDy);
            }

            // set camera pos
            if (Keyboard::key(sf::Keyboard::W)) {
                cameras[activeCamera]->updateCameraPos(CameraDirection::FORWARD, dt);
            }
            if (Keyboard::key(sf::Keyboard::S)) {
                cameras[activeCamera]->updateCameraPos(CameraDirection::BACKWARD, dt);
            }
            if (Keyboard::key(sf::Keyboard::D)) {
                cameras[activeCamera]->updateCameraPos(CameraDirection::RIGHT, dt);
            }
            if (Keyboard::key(sf::Keyboard::A)) {
                cameras[activeCamera]->updateCameraPos(CameraDirection::LEFT, dt);
            }
            if (Keyboard::key(sf::Keyboard::Space)) {
                cameras[activeCamera]->updateCameraPos(CameraDirection::UP, dt);
            }
            if (Keyboard::key(sf::Keyboard::LShift)) {
                cameras[activeCamera]->updateCameraPos(CameraDirection::DOWN, dt);
            }

            // set matrices
            view = cameras[activeCamera]->getViewMatrix();
            projection = Mat4x4::perspective(
                    radians(cameras[activeCamera]->getZoom()),    // FOV
                    (float) scrWidth / (float) scrHeight,                    // aspect ratio
                    0.1f, 100.0f                                        // near and far bounds
            );

            // set pos at end
            cameraPos = cameras[activeCamera]->cameraPos;
        }
    };

    // update screen before each frame
    void update() {
        glClearColor(bg[0], bg[1], bg[2], bg[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

    // update screen after frame
    void newFrame(Box &box) {
        box.positions.clear();
        box.sizes.clear();

        // process pending
        octree->processPending();
        octree->update(box);

        window.display();
    };

    // set uniform shader varaibles (lighting, etc)
    void renderShader(Shader shader, bool applyLighting = true) {
        // activate shader
        shader.activate();

        // set camera values
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.set3Float("viewPos", cameraPos);

        // lighting
        if (applyLighting) {
            // point lights
            unsigned int noLights = pointLights.size();
            unsigned int noActiveLights = 0;
            for (unsigned int i = 0; i < noLights; i++) {
                if (States::isActive(&activePointLights, i)) {
                    // i'th light is active
                    pointLights[i]->render(shader, noActiveLights);
                    noActiveLights++;
                }
            }
            shader.setInt("noPointLights", noActiveLights);

            // spot lights
            noLights = spotLights.size();
            noActiveLights = 0;
            for (unsigned int i = 0; i < noLights; i++) {
                if (States::isActive(&activeSpotLights, i)) {
                    // i'th spot light active
                    spotLights[i]->render(shader, noActiveLights);
                    noActiveLights++;
                }
            }
            shader.setInt("noSpotLights", noActiveLights);

            // directional light
            dirLight->render(shader);
        }
    };

    void renderInstances(std::string modelId, Shader shader, float dt) {
        models[modelId]->render(shader, dt, this);
    };

    /*
        cleanup method
    */
    void cleanup() {
        models.traverse([](Model *model) -> void {
            model->cleanup();
        });

        // clean up tries
        models.cleanup();
        instances.cleanup();

        // destroy octree
        octree->destroy();

        window.close();
    };

    /*
        accessors
    */
    bool shouldClose() {
        return !window.isOpen();
    };

    Camera *getActiveCamera() {
        return (activeCamera >= 0 && activeCamera < cameras.size()) ? cameras[activeCamera] : nullptr;
    };

    /*
        modifiers
    */
    void setShouldClose(bool shouldClose) {
        window.close();
    };

    void setWindowColor(float r, float g, float b, float a) {
        bg[0] = r;
        bg[1] = g;
        bg[2] = b;
        bg[3] = a;
    };

    /*
        Model/instance methods
    */
    void registerModel(Model *model) {
        models.insert(model->id, model);
    };

    RigidBody *generateInstance(std::string modelId, Vec3 size, float mass, Vec3 pos) {
        // generate new rigid body
        RigidBody *rb = models[modelId]->generateInstance(size, mass, pos);
        if (rb) {
            // successfully generated, set new and unique id for instance
            std::string id = generateId();
            rb->instanceId = id;
            // insert into trie
            instances.insert(id, rb);
            // insert into pending queue
            octree->addToPending(rb, models);
            return rb;
        }
        return nullptr;
    };

    void initInstances() {
        models.traverse([](Model *model) -> void {
            model->initInstances();
        });
    };

    void loadModels() {
        models.traverse([](Model *model) -> void {
            model->init();
        });
    };

    void removeInstance(std::string instanceId) {
        // get instance's model
        std::string targetModel = instances[instanceId]->modelId;

        // delete instance from model
        models[targetModel]->removeInstance(instanceId);

        // remove from trie
        instances[instanceId] = nullptr;
        instances.erase(instanceId);
    };

    void markForDeletion(std::string instanceId) {
        // activate kill switch
        States::activate(&instances[instanceId]->state, INSTANCE_DEAD);
        // push to list
        instancesToDelete.push_back(instances[instanceId]);
    };

    void clearDeadInstances() {
        for (RigidBody *rb: instancesToDelete) {
            removeInstance(rb->instanceId);
        }
        instancesToDelete.clear();
    };

    std::string currentId;

    std::string generateId() {
        for (int i = currentId.length() - 1; i >= 0; i--) {
            if ((int) currentId[i] != (int) 'z') {
                // increment then break
                currentId[i] = (char) (((int) currentId[i]) + 1);
                break;
            } else {
                currentId[i] = 'a';
            }
        }
        return currentId;
    };

    /*
     lights
 */

    // list of point lights
    std::vector<PointLight *> pointLights;
    // acts as array of switches for point lights
    unsigned int activePointLights;

    // list of spot lights
    std::vector<SpotLight *> spotLights;
    // acts as array of switches for spot lights
    unsigned int activeSpotLights;

    // direction light
    DirLight *dirLight;
    // switch for directional light
    bool dirLightActive;

    /*
        camera
    */

    // list of cameras
    std::vector<Camera *> cameras;
    // index of active camera
    unsigned int activeCamera;

    // camera position/matrices
    Mat4x4 view;
    Mat4x4 projection;
    Vec3 cameraPos;
    sf::RenderWindow window;
protected:
    sf::ContextSettings settings;

    // window vals
    const char *title;
    unsigned int scrWidth;
    unsigned int scrHeight;

    float bg[4]{}; // background color
};

#endif //CG_ENGINE_SCENE_H
