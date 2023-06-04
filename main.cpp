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
    glFrontFace(GL_CCW);


    int box_width, box_height, channels;
    unsigned char *data = stbi_load("res/textures/box.png", &box_width, &box_height, &channels, 0);

    const int verts = 36;

    float cube[] = {
            //position			normal					texture				color
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    ModelTransform polygonTrans1 = {Vec3(0.f, 0.f, 0.f),    // position
                                    Vec3(0.f, 0.f, 0.f),    // rotation
                                    Vec3(1.f, 1.f, 1.f)};    // scale

    ModelTransform polygonTrans2 = {Vec3(0.f, 0.f, 0.f),    // position
                                    Vec3(0.f, 0.f, 0.f),    // rotation
                                    Vec3(1.f, 1.f, 1.f)};    // scale

    ModelTransform polygonTrans3 = {Vec3(0.f, 0.f, 0.f),    // position
                                    Vec3(0.f, 0.f, 0.f),    // rotation
                                    Vec3(1.f, 1.f, 1.f)};    // scale

    ModelTransform lightTrans = {Vec3(0.f, 0.f, 0.f),    // position
                                 Vec3(0.f, 0.f, 0.f),    // rotation
                                 Vec3(0.1f, 0.1f, 0.1f)};    // scale

    unsigned int box_texture;
    glGenTextures(1, &box_texture);

    glBindTexture(GL_TEXTURE_2D, box_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    unsigned int VBO_polygon, VAO_polygon;
    glGenBuffers(1, &VBO_polygon);
    glGenVertexArrays(1, &VAO_polygon);

    glBindVertexArray(VAO_polygon);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    Shader *shader = new Shader("res/shader/vShader.glsl", "res/shader/fShader.glsl");
    Shader *light_shader = new Shader("res/shader/lCube.vert", "res/shader/lCube.frag");

    float dt = 0.0f;
    float lastFrame = 0.0f;


    Vec3 lightPos(0.0f, 0.0f, 0.0f);
    Vec3 lightColor(1.0f, 1.0f, 1.0f);
    Vec3 ambientColor(1.0f, 1.0f, 1.0f);

    sf::Clock clock;
    while (window.isOpen()) {
        float currentTime = clock.getElapsedTime().asSeconds();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window, dt);

        polygonTrans1.rotation.z = dt * 60.0;
        polygonTrans1.position.x = 0.6f * cos(dt * 0.5);
        polygonTrans1.position.y = 0.6f * sin(dt * 0.5);
        polygonTrans1.scale = Vec3(0.2f);


        polygonTrans2.rotation.z = dt * 30.0;
        polygonTrans2.position.x = 0.6f * cos(dt * 0.5 + 3.14158f);
        polygonTrans2.position.y = 0.6f * sin(dt * 0.5 + 3.14158f);
        polygonTrans2.scale = Vec3(0.2f);

        polygonTrans3.scale = Vec3(0.2f);

        lightPos.x = 2.0f * cos(dt * 1.2f);
        lightPos.y = 0.0f;
        lightPos.z = 2.0f * sin(dt * 1.2f);
        lightTrans.position = lightPos;


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Mat4x4 p = camera.GetProjectionMatrix();
        Mat4x4 v = camera.GetViewMatrix();
        Mat4x4 pv = p * v;

        Mat4x4 model = Mat4x4(1.f);

        model = model.translate(polygonTrans1.position);
        model = model.rotate(radians(polygonTrans1.rotation.x), Vec3(1.f, 0.f, 0.f));
        model = model.rotate(radians(polygonTrans1.rotation.y), Vec3(0.f, 1.f, 0.f));
        model = model.rotate(radians(polygonTrans1.rotation.z), Vec3(0.f, 0.f, 1.f));
        model = model.scale(polygonTrans1.scale);

        shader->use();
        shader->setMat4("pv", pv);
        shader->setMat4("model", model);
        shader->setBool("wireframeMode", wireframeMode);
        shader->set3Float("viewPos", camera.Position);
        shader->set3Float("lightPos", lightPos);
        shader->set3Float("lightColor", lightColor);
        shader->set3Float("ambientColor", ambientColor);

        glBindTexture(GL_TEXTURE_2D, box_texture);
        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 2

        model = Mat4x4(1.f);
        model = model.translate(polygonTrans2.position);
        model = model.rotate(radians(polygonTrans2.rotation.x), Vec3(1.f, 0.f, 0.f));
        model = model.rotate(radians(polygonTrans2.rotation.y), Vec3(0.f, 1.f, 0.f));
        model = model.rotate(radians(polygonTrans2.rotation.z), Vec3(0.f, 0.f, 1.f));
        model = model.scale(polygonTrans2.scale);

        shader->use();
        shader->setMat4("pv", pv);
        shader->setMat4("model", model);
        shader->setBool("wireframeMode", wireframeMode);
        shader->set3Float("viewPos", camera.Position);
        shader->set3Float("lightPos", lightPos);
        shader->set3Float("lightColor", lightColor);
        shader->set3Float("ambientColor", ambientColor);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 3

        model = Mat4x4(1.f);
        model = model.translate(polygonTrans3.position);
        model = model.rotate(radians(polygonTrans3.rotation.x), Vec3(1.f, 0.f, 0.f));
        model = model.rotate(radians(polygonTrans3.rotation.y), Vec3(0.f, 1.f, 0.f));
        model = model.rotate(radians(polygonTrans3.rotation.z), Vec3(0.f, 0.f, 1.f));
        model = model.scale(polygonTrans3.scale);

        shader->use();
        shader->setMat4("pv", pv);
        shader->setMat4("model", model);
        shader->setBool("wireframeMode", wireframeMode);
        shader->set3Float("viewPos", camera.Position);
        shader->set3Float("lightPos", lightPos);
        shader->set3Float("lightColor", lightColor);
        shader->set3Float("ambientColor", ambientColor);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // LIGHT
        model = Mat4x4(1.0f);
        model = model.translate(lightTrans.position);
        model = model.scale(lightTrans.scale);

        light_shader->use();
        light_shader->setMat4("pv", pv);
        light_shader->setMat4("model", model);
        light_shader->set3Float("lightColor", lightColor);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    delete light_shader;
    window.close();
    return 0;
}