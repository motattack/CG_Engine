#ifndef CG_ENGINE_SCENE_H
#define CG_ENGINE_SCENE_H

#include <vector>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <CG_Engine_Core/render/shader.h>
#include <CG_Engine_Core/UI/camera.h>
#include <CG_Engine_Core/math/vec3.h>
#include <CG_Engine_Core/render/light.h>
#include <CG_Engine_Core/algo/states.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <CG_Engine_Core/UI/mouse.h>
#include <CG_Engine_Core/UI/keyboard.h>

class Scene {
public:
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
    Scene() : window(sf::VideoMode(scrWidth, scrHeight, 32), "First Window",
                     sf::Style::Titlebar | sf::Style::Close, settings) {
    };

    Scene(const char *title, unsigned int scrWidth, unsigned int scrHeight) : title(title),
                                                                              activeCamera(-1),
                                                                              activePointLights(0),
                                                                              activeSpotLights(0) {


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

        return true;
    };

    /*
        main loop methods
    */
    // process input
    void processInput(float dt) {
        if (activeCamera != -1 && activeCamera < cameras.size()) {
            // active camera exists

            // set camera direction
            cameras[activeCamera]->updateCameraDirection(Mouse::getDX(), Mouse::getDY());

            // set camera zoom
            cameras[activeCamera]->updateCameraZoom(Mouse::getScrollDY());

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
    }


    // update screen before each frame
    void update() {
        glClearColor(bg[0], bg[1], bg[2], bg[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

    void newFrame() {
        window.display();
    }


    // set uniform shader varaibles (lighting, etc)
    void render(Shader shader, bool applyLighting = true) {
        // activate shader
        shader.activate();

        // set camera values
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.set4Float("viewPos", cameraPos);

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

    /*
        cleanup method
    */
    void cleanup() {
        window.close();
    };

    bool pollEvent(sf::Event &event) {
        return window.pollEvent(event);
    }

    /*
        accessors
    */
    bool shouldClose() {
        return !window.isOpen(); // Return true if the SFML window is closed
    }

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
        lights
    */
    // list of point lights
    std::vector<PointLight *> pointLights;
    unsigned int activePointLights{};
    // list of spot lights
    std::vector<SpotLight *> spotLights;
    unsigned int activeSpotLights{};
    // direction light
    DirLight *dirLight{};
    bool dirLightActive{};

    /*
        camera
    */
    std::vector<Camera *> cameras;
    unsigned int activeCamera{};
    Mat4x4 view;
    Mat4x4 projection;
    Mat4x4 cameraPos;

protected:
    // window object
    sf::RenderWindow window;
    sf::ContextSettings settings;

    // window vals
    const char *title{};
    unsigned int scrWidth = 800;
    unsigned int scrHeight = 600;

    float bg[4]{}; // background color
};


#endif //CG_ENGINE_SCENE_H