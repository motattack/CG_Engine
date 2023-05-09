#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <CG_Engine_Core/camera.h>
#include <CG_Engine_Core/model.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <stack>
#include "CG_Engine_Core/UI/screen.h"
#include "CG_Engine_Core/light.h"
#include "CG_Engine_Core/models/lamp.h"
#include "CG_Engine_Core/models/gun.h"
#include "CG_Engine_Core/models/sphere.h"
#include "CG_Engine_Core/physics/evn.h"
#include "CG_Engine_Core/models/box.h"

void onResize(const sf::Event &event); // Protype
void userInput(sf::Window &window, float dt);

// Matrix's
Mat4x4 projection;
Mat4x4 model;
Mat4x4 view;

Screen screen;

bool flashlightOn = false;
double dt = 0.0f; // tme btwn frames

// Camera
Camera Camera::defaultCamera(Vec3(0.0f, 0.0f, 0.0f));

// Frames
float lastFrame = 0.0f;

SphereArray launchObjects;


int main() {
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Error: glew not init =(" << std::endl;
        return -1;
    }
    //glEnable(GL_DEPTH_TEST);

    if (!screen.init()) {
        std::cout << "Could not open window" << std::endl;
        return -1;
    }

    screen.setParameters();

    // SHADERS===============================
    Shader shader("res/assets/object.vs", "res/assets/object.fs");
    Shader lampShader("res/assets/instanced/instanced.vs", "res/assets/lamp.fs");
    Shader launchShader("res/assets/instanced/instanced.vs", "res/assets/object.fs");
    Shader boxShader("res/assets/instanced/box.vs", "res/assets/instanced/box.fs");

    // MODELS==============================
    launchObjects.init();

    Box box;
    box.init();

    Model m(BoundTypes::AABB, Vec3(0.0f), Vec3(0.05f));
    m.loadModel("res/assets/models/lotr_troll/scene.gltf");

    // LIGHTS
    DirLight dirLight = {Vec3(-0.2f, -1.0f, -0.3f), Vec4(0.1f, 0.1f, 0.1f, 1.0f), Vec4(0.4f, 0.4f, 0.4f, 1.0f),
                         Vec4(0.5f, 0.5f, 0.5f, 1.0f)};

    Vec3 pointLightPositions[] = {
            Vec3(0.7f, 0.2f, 2.0f),
            Vec3(2.3f, -3.3f, -4.0f),
            Vec3(-4.0f, 2.0f, -12.0f),
            Vec3(0.0f, 0.0f, -3.0f)
    };

    Vec4 ambient = Vec4(0.05f, 0.05f, 0.05f, 1.0f);
    Vec4 diffuse = Vec4(0.8f, 0.8f, 0.8f, 1.0f);
    Vec4 specular = Vec4(1.0f);
    float k0 = 1.0f;
    float k1 = 0.09f;
    float k2 = 0.032f;

    LampArray lamps;
    lamps.init();
    for (auto &pointLightPosition: pointLightPositions) {
        lamps.lightInstances.push_back(
                {
                        pointLightPosition,
                        k0, k1, k2,
                        ambient, diffuse, specular
                });
    }

    SpotLight s = {
            Camera::defaultCamera.cameraPos, Camera::defaultCamera.cameraFront,
            cos(radians(12.5f)), cos(radians(20.0f)),
            1.0f, 0.07f, 0.032f,
            Vec4(0.0f, 0.0f, 0.0f, 1.0f), Vec4(1.0f), Vec4(1.0f)
    };

    sf::Clock deltaClock, clock;
    while (screen.window.isOpen()) {
        box.offsets.clear();
        box.sizes.clear();

        // calculate dt
        float currentTime = clock.getElapsedTime().asSeconds();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;

        // process input
        userInput(screen.window, dt);

        // render
        screen.update();

        // draw shapes
        shader.activate();
        launchShader.activate();

        // camera view position
        shader.activate();
        shader.set3Float("viewPos", Camera::defaultCamera.cameraPos);
        launchShader.activate();
        launchShader.set3Float("viewPos", Camera::defaultCamera.cameraPos);


        // render lights in shader
        shader.activate();
        dirLight.render(shader);
        launchShader.activate();
        dirLight.render(launchShader);

        for (unsigned int i = 0; i < 4; i++) {
            shader.activate();
            lamps.lightInstances[i].render(shader, i);
            launchShader.activate();
            lamps.lightInstances[i].render(launchShader, i);
        }
        shader.activate();
        shader.setInt("noPointLights", 4);
        launchShader.activate();
        launchShader.setInt("noPointLights", 4);

        if (flashlightOn) {
            s.position = Camera::defaultCamera.cameraPos;
            s.direction = Camera::defaultCamera.cameraFront;
            shader.activate();
            s.render(shader, 0);
            shader.setInt("noSpotLights", 1);
            launchShader.activate();
            s.render(launchShader, 0);
            launchShader.setInt("noSpotLights", 1);
        }
        else {
            shader.activate();
            shader.setInt("noSpotLights", 0);
            launchShader.activate();
            launchShader.setInt("noSpotLights", 0);
        }

        // create transformation
        view = Mat4x4(1.0f);
        projection = Mat4x4(1.0f);
        view = Camera::defaultCamera.getViewMatrix();
        projection = Mat4x4::perspective(
                radians(Camera::defaultCamera.zoom),
                800 / 600, 0.1f, 100.0f);

        shader.activate();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        m.render(shader, dt, &box);

        // launch objects
        std::stack<int> removeObjects;
        for (int i = 0; i < launchObjects.instances.size(); i++) {
            if ((Camera::defaultCamera.cameraPos - launchObjects.instances[i].pos).len() > 50.0f) {
                removeObjects.push(i);
                continue;
            }
        }
        for (int i = 0; i < removeObjects.size(); i++) {
            launchObjects.instances.erase(launchObjects.instances.begin() + removeObjects.top());
            removeObjects.pop();
        }

        if (launchObjects.instances.size() > 0) {
            launchShader.activate();
            launchShader.setMat4("view", view);
            launchShader.setMat4("projection", projection);
            launchObjects.render(launchShader, dt, &box);
        }

        // lamps
        lampShader.activate();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        lamps.render(lampShader, dt, &box);

        // render boxes
        if (box.offsets.size() > 0) {
            // instances exist
            boxShader.activate();
            boxShader.setMat4("view", view);
            boxShader.setMat4("projection", projection);
            box.render(boxShader);
        }

        screen.window.display();

        // Poll events
        sf::Event event{};
        while (screen.window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                screen.window.close();
            else if (event.type == sf::Event::Resized)
                onResize(event);
        }
    }

    lamps.cleanup();
    box.cleanup();
    launchObjects.cleanup();
    m.cleanup();

    ImGui::SFML::Shutdown();
    screen.window.close();
    return 0;
}

void onResize(const sf::Event &event) {
    glViewport(0, 0, event.size.width, event.size.height);
}

void launchItem(float dt) {
    RigidBody rb(1.0f, Camera::defaultCamera.cameraPos);
    rb.transferEnergy(100.0f, Camera::defaultCamera.cameraFront);
    rb.applyAcceleration(Environment::gravitationalAcceleration);
    launchObjects.instances.push_back(rb);
}

void userInput(sf::Window &window, float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        flashlightOn = !flashlightOn;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        Camera::defaultCamera.updateCameraPos(CameraDirection::FORWARD, dt);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        Camera::defaultCamera.updateCameraPos(CameraDirection::BACKWARD, dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        Camera::defaultCamera.updateCameraPos(CameraDirection::RIGHT, dt);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        Camera::defaultCamera.updateCameraPos(CameraDirection::LEFT, dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        Camera::defaultCamera.updateCameraPos(CameraDirection::UP, dt);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        Camera::defaultCamera.updateCameraPos(CameraDirection::DOWN, dt);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        Camera::defaultCamera.mouseCursorPosition(sf::Mouse::getPosition(window), window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        launchItem(dt);
}