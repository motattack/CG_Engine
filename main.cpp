#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "SFML/Graphics/RenderWindow.hpp"
#include <cmath>
#include <shader.h>
#include <vbuffer.h>
#include <varray.h>
#include <vec3.h>
#include <mat4x4.h>
#include <common.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#define SCR_WIDTH 1200
#define SCR_HEIGHT 800

using namespace std;

// pos and color and TexCoords
float vertex[] = {
        /* Top Position */        /* Color */            /* TexCoords */
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        /* Bottom Position */        /* Color */            /* TexCoords */
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        /* Left Position */        /* Color */            /* TexCoords */
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        /* Right Position */        /* Color */            /* TexCoords */
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        /* Back Position */        /* Color */            /* TexCoords */
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        /* Front Position */        /* Color */            /* TexCoords */
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};

Vec3 cubePositions[] = {
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(2.0f, 5.0f, -15.0f),
        Vec3(-1.5f, -2.2f, -2.5f),
        Vec3(-3.8f, -2.0f, -12.3f),
        Vec3(2.4f, -0.4f, -3.5f),
        Vec3(-1.7f, 3.0f, -7.5f),
        Vec3(1.3f, -2.0f, -2.5f),
        Vec3(1.5f, 2.0f, -2.5f),
        Vec3(1.5f, 0.2f, -1.5f),
        Vec3(-1.3f, 1.0f, -1.5f)
};

void onResize(const sf::Event &event); // Protype
void userInput(sf::Window &window);

void mouseCursorPosition(const sf::Event &event); // Protype
void mouseScrollCallback(const sf::Event &event);

unsigned int loadTexture(const char *texture_path);

// Matrix's
Mat4x4 projection;
Mat4x4 model;
Mat4x4 view;

// Vectors
Vec3 cameraPos = Vec3(0.0f, 0.0f, 3.0f);
Vec3 cameraFront = Vec3(0.0f, 0.0f, -1.0f);
Vec3 cameraUp = Vec3(0.0f, 1.0f, 0.0f);

// Camera Attr
float pitch = 0.0f;
float yaw = -120.0f;
float lastX = float(SCR_WIDTH) / 2.0f;
float lastY = float(SCR_HEIGHT) / 2.0f;
bool isFirstMouse = true;
float zoom = 45.0f;


// Frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 4;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT, 32), "First Window",
                            sf::Style::Titlebar | sf::Style::Close, settings);
    window.setActive(true);
    window.setMouseCursorVisible(false);

    glewExperimental = GL_TRUE;

    // Check Glew
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cout << "Error:: glew not init =(: " << glewGetErrorString(glewError) << std::endl;
        return -1;
    }

    // OpenGL Settings;
    glEnable(GL_DEPTH_TEST);

    /* Buffers */

    vBuffer VBO(vertex, sizeof(vertex) / sizeof(float));
    vArray VAO;
    vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), &vertex, GL_STATIC_DRAW);

    /* Position Attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    /* Color Attribute*/
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Textue Coordinates Attribute */
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /* Texture */
    stbi_set_flip_vertically_on_load(true);
    GLuint container_texture = loadTexture(
            "../res/texture/example/head.png");
    GLuint face_texture = loadTexture(
            "../res/texture/example/head2.png");


    /* Shader */
    Shader myShader("../res/shader/vShader.glsl",
                    "../res/shader/fShader.glsl");
    myShader.use();
    myShader.setInt("container_texture", 0);
    myShader.setInt("face_texture", 1);

    sf::Clock clock;
    while (window.isOpen()) {
        /* Update */
        userInput(window);
        float time = clock.getElapsedTime().asSeconds();

        deltaTime = time - lastFrame;
        lastFrame = time;

        float xValue = std::cos(time) / 2.0f + 0.5f; // 0.0f - 1.0f
        float yValue = std::sin(time) / 2.0f + 0.5f; // 0.0f - 1.0f

        // Vector
        Vec3 myVector;
        myVector.x = xValue;
        myVector.y = yValue;
        myVector.z = 0.31f;

        myShader.setVec3("colors", myVector);
        myShader.setFloat("alpha", xValue);

        /* Coordinates */
        // Projection
        projection = Mat4x4::perspective(radians(zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
        myShader.setMat4x4("projection", projection);

        // View
        view = Mat4x4(1.0f);
        view = Mat4x4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        myShader.setMat4x4("view", view);

        // Model
        model = Mat4x4(1.0f);
        model = model.Scale(Vec3(1.2f));
        myShader.setMat4x4("model", model);


        /* Render */
        glClearColor(0.7f, 0.7f, 7.0f, 0.0f); // 0.0f - 1.0f
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glLineWidth(7.0f);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // hehe

        // One shape
        myShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, face_texture);
        VAO.bind();

        // 1 elem
        myShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, face_texture);
        VAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Display
        window.display();

        // Poll events
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
                onResize(event);
            else if (event.type == sf::Event::MouseMoved)
                mouseCursorPosition(event);
            else if (event.type == sf::Event::MouseWheelScrolled)
                mouseScrollCallback(event);
        }
    }
    window.close();
    return 0;
}

void onResize(const sf::Event &event) {
    glViewport(0, 0, event.size.width, event.size.height);
}

void userInput(sf::Window &window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

    const float cameraSpeed = 3.0f * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        cameraPos += cameraFront * cameraSpeed;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        cameraPos -= cameraFront * cameraSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        cameraPos += cameraFront.crossProduct(cameraUp).normalize() * cameraSpeed;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        cameraPos -= cameraFront.crossProduct(cameraUp).normalize() * cameraSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        cameraPos += cameraUp * cameraSpeed;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        cameraPos -= cameraUp * cameraSpeed;
}

void mouseCursorPosition(const sf::Event &event) {
    float xpos = event.mouseMove.x;
    float ypos = event.mouseMove.y;

    if (isFirstMouse) {
        lastX = xpos;
        lastY = ypos;
        isFirstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch >= 119.0f)
        pitch = 119.0f;
    if (pitch <= -119.0f)
        pitch = -119.0f;

    Vec3 direction;
    direction.x = std::cos(radians(yaw)) * std::cos(radians(pitch));
    direction.y = std::sin(radians(pitch));
    direction.z = std::sin(radians(yaw)) * std::cos(radians(pitch));
    cameraFront = direction.normalize();
}

void mouseScrollCallback(const sf::Event &event) {
    zoom -= float(event.mouseWheelScroll.delta);
    if (zoom >= 45.0f)
        zoom = 45.0f;
    if (zoom <= 20.0f)
        zoom = 20.0f;
}

unsigned int loadTexture(const char *texture_path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    /* Filter Options */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texture_path, &width, &height, &nrChannels, 0);

    if (data) {
        // Note it's a better way to see that what our file is like png, jpg or jpeg ?
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        if (nrChannels == 3) // jpg file
            format = GL_RGB;
        if (nrChannels == 4) // png file
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Fail load texture\n";
    }

    stbi_image_free(data);

    return textureID;
}