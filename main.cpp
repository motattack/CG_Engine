#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <CG_Engine/ui/camera.h>
#include <CG_Engine/ui/keyboard.h>
#include <CG_Engine/ui/mouse.h>
#include <CG_Engine/objects/model.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

Camera camera(Vec3(0.183165, -0.0376139, 0.031249), Vec3(0.f, 1.0f, 0.f), 243.051, -20.7);

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
}

// Matrix's
Mat4x4 projection;
Mat4x4 model;
Mat4x4 view;

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

    Vec3 lightPos = Vec3(1.2f, 1.0f, 1.5f);

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

    /* Shader */
    Shader myShader("res/shader/vShader.glsl",
                    "res/shader/fShader.glsl");
    myShader.use();
    Shader lightCubeShader("res/Shader/lCube.vert", "res/Shader/lCube.frag");

    stbi_set_flip_vertically_on_load(true);
    Model myBackPack("res/models/backpack/backpack.obj");

    float dt;
    float lastFrame = 0.0f;

    sf::Clock clock;
    while (window.isOpen()) {
        float currentTime = clock.getElapsedTime().asSeconds();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window, dt);

        float radius = 5.0f;
        float camX = std::sin(currentTime) * radius;
        float camZ = std::cos(currentTime) * radius;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();

        lightPos = Vec3(camX, camZ, 0.0f);
        myShader.set3Float("light.position", camera.Position);
        myShader.set3Float("light.direction", camera.Front);
        myShader.set3Float("light.ambient", Vec3(0.2f));
        myShader.set3Float("light.diffuse", Vec3(0.4f));
        myShader.set3Float("light.specular", Vec3(0.5f));
        myShader.set3Float("viewPos", camera.Position);

        /* Coordinates */
        // Projection
        projection = camera.GetProjectionMatrix();
        myShader.setMat4("projection", projection);

        // View
        view = Mat4x4(1.0f);
        view = camera.GetViewMatrix();
        myShader.setMat4("view", view);

        // Model
        model = Mat4x4(1.0f);
        myShader.setMat4("model", model);

        // Draw your model
        myBackPack.Draw(myShader);

        // Second Object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = Mat4x4(1.0f);
        model = model.translate(lightPos);
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        myBackPack.Draw(myShader);

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
    window.close();
    return 0;
}