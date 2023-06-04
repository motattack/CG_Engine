#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/math/common.h>
#include <CG_Engine/UI/camera.h>
#include <CG_Engine/UI/keyboard.h>
#include <CG_Engine/UI/mouse.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

Camera camera(Vec3(0.f, 0.f, -2.f));

bool wireframeMode = false;

void setPolygonMode() {
    if (wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void processInput(sf::Window &window, float dt) {
    if (Keyboard::key(sf::Keyboard::W)) {
        camera.Move(CameraDirection::FORWARD, dt);
    }
    if (Keyboard::key(sf::Keyboard::S)) {
        camera.Move(CameraDirection::BACKWARD, dt);
    }
    if (Keyboard::key(sf::Keyboard::D)) {
        camera.Move(CameraDirection::RIGHT, dt);
    }
    if (Keyboard::key(sf::Keyboard::A)) {
        camera.Move(CameraDirection::LEFT, dt);
    }
    if (Keyboard::key(sf::Keyboard::Space)) {
        camera.Move(CameraDirection::UP, dt);
    }
    if (Keyboard::key(sf::Keyboard::LShift)) {
        camera.Move(CameraDirection::DOWN, dt);
    }

    camera.Rotate(Mouse::getDX(), Mouse::getDY());

    camera.ChangeFOV(Mouse::getScrollDY());

    if (Keyboard::key(sf::Keyboard::Escape)) {
        window.close();
    }

    if (Keyboard::keyWentUp(sf::Keyboard::E)) {
        wireframeMode = !wireframeMode;
        setPolygonMode();
    }
};

struct ModelTransform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
};

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(1280, 720, 32), "GL_Engine",
                      sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(75);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Error:: glew not init =(" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    setPolygonMode();
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);


    int box_width, box_height, channels;
    unsigned char *data = stbi_load("res/textures/box.png", &box_width, &box_height, &channels, 0);

    const int verts = 8;

    float cube[verts * (3 + 3 + 2)] = {
            /*        position       */   /*             color      */ /* texture */
/*1*/    -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.f, 1.f,
/*2*/    1.0f, 1.0f, -1.0f, 0.5f, 0.5f, 0.0f, 1.f, 1.f,
/*3*/    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,
/*4*/    -1.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.f, 0.f,
/*5*/    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.f, 0.f,
/*6*/    1.0f, -1.0f, -1.0f, 0.5f, 0.0f, 0.5f, 0.f, 0.f,
/*7*/    1.0f, -1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.f, 1.f,
/*8*/    -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.f, 1.f
    };

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3,
            0, 4, 1,
            1, 4, 5,
            0, 3, 7,
            0, 7, 4,
            1, 6, 2,
            1, 5, 6,
            2, 7, 3,
            2, 6, 7,
            4, 7, 5,
            5, 7, 6
    };

    ModelTransform polygonTrans = {Vec3(0.f, 0.f, 0.f),
                                   Vec3(0.f, 0.f, 0.f),
                                   Vec3(1.f, 1.f, 1.f)};

    unsigned int box_texture;
    glGenTextures(1, &box_texture);

    glBindTexture(GL_TEXTURE_2D, box_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGB,  GL_UNSIGNED_BYTE, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts * 8, cube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Shader *shader = new Shader("res/shader/vShader.glsl", "res/shader/fShader.glsl");

    bool isGo = true;
    float dt = 0.0f;
    float lastFrame = 0.0f;

    float dist = 5.f;

    sf::Clock clock;
    while (window.isOpen()) {
        float currentTime = clock.getElapsedTime().asSeconds();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window, dt);

        polygonTrans.rotation.z = dt * 60.f;
//        polygonTrans.rotation.x = dt * 45.f;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        shader->use();

        Mat4x4 model = Mat4x4(1.f);
        model = model.translate(polygonTrans.position);
        model = model.rotate(radians(polygonTrans.rotation.x), Vec3(1.f, 0.f, 0.f));
        model = model.rotate(radians(polygonTrans.rotation.y), Vec3(0.f, 1.f, 0.f));
        model = model.rotate(radians(polygonTrans.rotation.z), Vec3(0.f, 0.f, 1.f));
        model = model.scale(Vec3(0.2f));

        Mat4x4 pvm = camera.GetProjectionMatrix() * camera.GetViewMatrix() * model;

        shader->set4Float("pvm", pvm);
        shader->setBool("wireframeMode", wireframeMode);

        glBindTexture(GL_TEXTURE_2D, box_texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        window.display();


        sf::Event event{};
        while (window.pollEvent(event)) {
            Keyboard::keyCallback(event);
            Mouse::mouseWheelCallback(event.mouseWheelScroll.delta);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    Mouse::cursorPosCallback(event.mouseMove.x, event.mouseMove.y);
                    break;
                case sf::Event::MouseButtonPressed:
                    Mouse::mouseButtonCallback(event.mouseButton.button, event.type);
                    break;
                default:
                    break;
            }
        }
    }
    delete shader;
    window.close();
    return 0;
}