#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <CG_Engine_Core/UI/camera.h>
#include <CG_Engine_Core/model.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <stack>
#include "CG_Engine_Core/light.h"
#include "CG_Engine_Core/models/lamp.h"
#include "CG_Engine_Core/models/gun.h"
#include "CG_Engine_Core/models/sphere.h"
#include "CG_Engine_Core/physics/evn.h"
#include "CG_Engine_Core/models/box.h"
#include "scene.h"

void onResize(const sf::Event &event); // Protype
void userInput(Scene &window, float dt);

// Matrix's
Mat4x4 projection;
Mat4x4 model;
Mat4x4 view;

//Screen screen;

bool flashlightOn = false;
double dt = 0.0f; // tme btwn frames

// Camera
Camera cam;

// Frames
float lastFrame = 0.0f;

SphereArray launchObjects;


int main() {
    Scene scene = Scene("OpenGL Tutorial", 800, 600);

    if (!scene.init()) {
        std::cout << "Could not open window" << std::endl;
        return -1;
    }

    scene.cameras.push_back(&cam);
    scene.activeCamera = 0;

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
    scene.dirLight = &dirLight;

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

    PointLight pointLights[4];

    LampArray lamps;
    lamps.init();
    for (unsigned int i = 0; i < 4; i++) {
        pointLights[i] = {
                pointLightPositions[i],
                k0, k1, k2,
                ambient, diffuse, specular
        };
        lamps.lightInstances.push_back(pointLights[i]);
        scene.pointLights.push_back(&pointLights[i]);
        States::activate(&scene.activePointLights, i);
    }

    SpotLight spotLight = {
            cam.cameraPos, cam.cameraFront,
            cos(radians(12.5f)), cos(radians(20.0f)),
            1.0f, 0.07f, 0.032f,
            Vec4(0.0f, 0.0f, 0.0f, 1.0f), Vec4(1.0f), Vec4(1.0f)
    };
    scene.spotLights.push_back(&spotLight);
    scene.activeSpotLights = 1;

    sf::Clock deltaClock, clock;
    while (scene.window.isOpen()) {
        box.positions.clear();
        box.sizes.clear();

        // calculate dt
        float currentTime = clock.getElapsedTime().asSeconds();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;

        // process input
        userInput(scene, dt);

        // update screen values
        scene.update();

        scene.render(shader);
        m.render(shader, dt, &box);

        // launch objects
        std::stack<int> removeObjects;
        for (int i = 0; i < launchObjects.instances.size(); i++) {
            if ((scene.getActiveCamera()->cameraPos - launchObjects.instances[i].pos).len() > 50.0f) {
                removeObjects.push(i);
                continue;
            }
        }
        for (int i = 0; i < removeObjects.size(); i++) {
            launchObjects.instances.erase(launchObjects.instances.begin() + removeObjects.top());
            removeObjects.pop();
        }

        if (launchObjects.instances.size() > 0) {
            scene.render(launchShader);
            launchObjects.render(launchShader, dt, &box);
        }

        // lamps
        scene.render(lampShader, false);
        lamps.render(lampShader, dt, &box);

        // render boxes
        if (box.positions.size() > 0) {
            // instances exist
            scene.render(boxShader, false);
            box.render(boxShader);
        }

        // send new frame to window
        scene.newFrame();

        // Poll events
        sf::Event event{};
        while (scene.window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            Keyboard::keyCallback(event);
            if (event.type == sf::Event::Closed)
                scene.window.close();
            else if (event.type == sf::Event::Resized)
                onResize(event);
        }
    }

    lamps.cleanup();
    box.cleanup();
    launchObjects.cleanup();
    m.cleanup();

    ImGui::SFML::Shutdown();
    scene.window.close();
    return 0;
}

void onResize(const sf::Event &event) {
    glViewport(0, 0, event.size.width, event.size.height);
}

void launchItem(float dt) {
    RigidBody rb(1.0f, cam.cameraPos);
    rb.transferEnergy(100.0f, cam.cameraFront);
    rb.applyAcceleration(Environment::gravitationalAcceleration);
    launchObjects.instances.push_back(rb);
}

void userInput(Scene &scene, float dt) {
    scene.processInput(dt);

    // update flash light
    if (States::isActive(&scene.activeSpotLights, 0)) {
        scene.spotLights[0]->position = scene.getActiveCamera()->cameraPos;
        scene.spotLights[0]->direction = scene.getActiveCamera()->cameraFront;
    }

    if (Keyboard::key(sf::Keyboard::Escape)) {
        scene.setShouldClose(true);
    }

    if (Keyboard::key(sf::Keyboard::L)) {
        States::toggle(&scene.activeSpotLights, 0); // toggle spot light
    }

    if (Keyboard::key(sf::Keyboard::F))
        launchItem(dt);

    for (int i = 0; i < 4; i++) {
        if (Keyboard::key(static_cast<sf::Keyboard::Key>(sf::Keyboard::Num1 + i))) {
            States::toggle(&scene.activePointLights, i);
        }
    }
}