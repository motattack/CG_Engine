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

struct Material {
    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
    float shininess;
};

struct DirecationalLight {
    Vec3 direction;

    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
};

struct PointLight {
    Vec3 position;

    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;

    float constant;
    float linear;
    float quadratic;
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

    Material cubeMaterials[3] = {
            {
                    Vec3(0.25, 0.20725, 0.20725),
                    Vec3(1, 0.829, 0.829),
                    Vec3(0.296648, 0.296648, 0.296648),
                    12.f
            }, // pearl
            {
                    Vec3(0.25, 0.25, 0.25),
                    Vec3(0.4, 0.4, 0.4),
                    Vec3(0.774597, 0.774597, 0.774597),
                    77.f
            }, // chrome
            {
                    Vec3(0.1745, 0.01175, 0.01175),
                    Vec3(0.61424, 0.04136, 0.04136),
                    Vec3(0.727811, 0.626959, 0.626959),
                    77.f
            } // ruby
    };

    const int cube_count = 100;

    ModelTransform cubeTrans[cube_count];
    int cubeMat[cube_count];
    for (int i = 0; i < cube_count; i++) {
        float scale = (rand() % 6 + 1) / 20.0f;
        cubeTrans[i] = {
                Vec3((rand() % 201 - 100) / 50.0f, (rand() % 201 - 100) / 50.0f, (rand() % 201 - 100) / 50.0f),
                Vec3(rand() / 100.0f, rand() / 100.0f, rand() / 100.0f),
                Vec3(scale, scale, scale)
        };
        cubeMat[i] = rand() % 3;
    }

    ModelTransform lightTrans = {
            Vec3(0.f, 0.f, 0.f),    // position
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    Shader *shader = new Shader("res/shader/vShader.glsl", "res/shader/fShader.glsl");
    Shader *light_shader = new Shader("res/shader/lCube.vert", "res/shader/lCube.frag");

    float dt = 0.0f;
    float lastFrame = 0.0f;


    PointLight light1 = {Vec3(0.0f, 0.0f, 0.0f),
                         Vec3(0.4f, 0.4f, 0.4f),
                         Vec3(1.0f, 1.0f, 1.0f),
                         Vec3(3.0f, 3.0f, 3.0f),
                         0.9f, 0.1f, 0.09f};

    sf::Clock clock;
    while (window.isOpen()) {
        float currentTime = clock.getElapsedTime().asSeconds();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window, dt);

        lightTrans.position = light1.position;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Mat4x4 p = camera.GetProjectionMatrix();
        Mat4x4 v = camera.GetViewMatrix();
        Mat4x4 pv = p * v;

        Mat4x4 model = Mat4x4(1.f);

        for (int i = 0; i < cube_count; i++) {
            model = Mat4x4(1.0f);

            model = model.translate(cubeTrans[i].position);
            model = model.rotate(radians(cubeTrans[i].rotation.x), Vec3(1.f, 0.f, 0.f));
            model = model.rotate(radians(cubeTrans[i].rotation.y), Vec3(0.f, 1.f, 0.f));
            model = model.rotate(radians(cubeTrans[i].rotation.z), Vec3(0.f, 0.f, 1.f));
            model = model.scale(cubeTrans[i].scale);

            shader->use();
            shader->setMat4("pv", pv);
            shader->setMat4("model", model);
            shader->setBool("wireframeMode", wireframeMode);
            shader->set3Float("viewPos", camera.Position);

            shader->set3Float("light.position", light1.position);
            shader->set3Float("light.ambient", light1.ambient);
            shader->set3Float("light.diffuse", light1.diffuse);
            shader->set3Float("light.specular", light1.specular);
            shader->setFloat("light.constant", light1.constant);
            shader->setFloat("light.linear", light1.linear);
            shader->setFloat("light.quadratic", light1.quadratic);

            shader->set3Float("material.ambient", cubeMaterials[cubeMat[i]].ambient);
            shader->set3Float("material.diffuse", cubeMaterials[cubeMat[i]].diffuse);
            shader->set3Float("material.specular", cubeMaterials[cubeMat[i]].specular);
            shader->setFloat("material.shininess", cubeMaterials[cubeMat[i]].shininess);


            glBindTexture(GL_TEXTURE_2D, box_texture);
            glBindVertexArray(VAO_polygon);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // LIGHT
        model = Mat4x4(1.0f);
        model = model.translate(lightTrans.position);
        model = model.scale(lightTrans.scale);

        light_shader->use();
        light_shader->setMat4("pv", pv);
        light_shader->setMat4("model", model);
        light_shader->set3Float("lightColor", light1.specular);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window.display();


        sf::Event event{};
        while (window.pollEvent(event)) {
            Keyboard::keyCallback(event);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    Mouse::cursorPosCallback(event.mouseMove.x, event.mouseMove.y);
                    break;
                case sf::Event::MouseWheelScrolled:
                    Mouse::mouseWheelCallback(event.mouseWheelScroll.delta);
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