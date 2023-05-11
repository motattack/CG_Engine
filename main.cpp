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

void userInput(Scene &window, float dt);

// Camera
Camera cam;

double dt = 0.0f; // tme btwn frames
float lastFrame = 0.0f;

Sphere sphere(10);


int main() {
    Scene scene = Scene("OpenGL Tutorial", 800, 600);

    if (!scene.init()) {
        std::cout << "Could not open window" << std::endl;
        scene.cleanup();
        return -1;
    }

    scene.cameras.push_back(&cam);
    scene.activeCamera = 0;

    // SHADERS===============================
    Shader lampShader("res/assets/instanced/instanced.vs", "res/assets/lamp.fs");
    Shader shader("res/assets/instanced/instanced.vs", "res/assets/object.fs");
    Shader boxShader("res/assets/instanced/box.vs", "res/assets/instanced/box.fs");

    // MODELS==============================
    Lamp lamp(4);
    scene.registerModel(&lamp);

    scene.registerModel(&sphere);

    Box box;
    box.init();

    // load all model data
    scene.loadModels();

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

    for (unsigned int i = 0; i < 4; i++) {
        pointLights[i] = {
                pointLightPositions[i],
                k0, k1, k2,
                ambient, diffuse, specular
        };
        scene.generateInstance(lamp.id, Vec3(0.25f), 0.25f, pointLightPositions[i]);
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
    scene.activeSpotLights = 1;    // 0b00000001

    // instantiate instances
    scene.initInstances();

    // joystick recognition
    /*mainJ.update();
    if (mainJ.isPresent()) {
        std::cout << mainJ.getName() << " is present." << std::endl;
    }*/

    scene.prepare(box);

    sf::Clock deltaClock, clock;
    while (scene.window.isOpen()) {
        // calculate dt
        float currentTime = clock.getElapsedTime().asSeconds();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;

        // update screen values
        scene.update();

        // process input
        userInput(scene, dt);

        // remove launch objects if too far
        for (int i = 0; i < sphere.currentNoInstances; i++) {
            if ((cam.cameraPos - sphere.instances[i]->pos).len() > 250.0f) {
                scene.markForDeletion(sphere.instances[i]->instanceId);
            }
        }

        // render launch objects
        if (sphere.currentNoInstances > 0) {
            scene.renderShader(shader);
            scene.renderInstances(sphere.id, shader, dt);
        }

        // render lamps
        scene.renderShader(lampShader, false);
        scene.renderInstances(lamp.id, lampShader, dt);

        // render boxes
        scene.renderShader(boxShader, false);
        box.render(boxShader);

        // send new frame to window
        scene.newFrame(box);


        // Poll events
        sf::Event event{};
        while (scene.window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            Keyboard::keyCallback(event);
            if (event.type == sf::Event::Closed)
                scene.window.close();
        }

        scene.clearDeadInstances();
    }

    scene.cleanup();
    return 0;
}

void launchItem(Scene &scene) {
    RigidBody *rb = scene.generateInstance(sphere.id, Vec3(0.1f), 1.0f, cam.cameraPos);
    if (rb) {
        // instance generated
        rb->transferEnergy(1000.0f, cam.cameraFront);
        rb->applyAcceleration(Environment::gravitationalAcceleration);
    }
}

void userInput(Scene &scene, float dt) {
    scene.processInput(dt);

//    // update flash light
//    if (States::isIndexActive(&scene.activeSpotLights, 0)) {
//        scene.spotLights[0]->position = scene.getActiveCamera()->cameraPos;
//        scene.spotLights[0]->direction = scene.getActiveCamera()->cameraFront;
//    }

    if (Keyboard::key(sf::Keyboard::Escape)) {
        scene.setShouldClose(true);
    }

    if (Keyboard::key(sf::Keyboard::L)) {
        States::toggleIndex(&scene.activeSpotLights, 0); // toggle spot light
    }

    if (Keyboard::key(sf::Keyboard::F))
        launchItem(scene);

    for (int i = 0; i < 4; i++) {
        if (Keyboard::key(static_cast<sf::Keyboard::Key>(sf::Keyboard::Num1 + i))) {
            States::toggleIndex(&scene.activePointLights, i);
        }
    }
}