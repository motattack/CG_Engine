#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
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

void onResize(const sf::Event &event); // Protype
void userInput(sf::Window &window);

void mouseCursorPosition(const sf::Event &event); // Protype
void mouseScrollCallback(const sf::Event &event);

unsigned int loadTexture(const char *texture_path);

Mat4x4 projection;
Mat4x4 model;
Mat4x4 view;

auto myPos = Vec3(0.0f);


int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 4;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "Hello World");
    window.setActive(true);

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
    GLuint texture_head_1 = loadTexture("C:/Users/motattack/CLionProjects/CG_Engine/src/libs/texture/example/head.png");
    GLuint texture_head_2 = loadTexture(
            "C:/Users/motattack/CLionProjects/CG_Engine/src/libs/texture/example/head2.png");


    /* Shader */
    Shader myShader("C:/Users/motattack/CLionProjects/CG_Engine/src/libs/shader/exec/vShader.glsl",
                    "C:/Users/motattack/CLionProjects/CG_Engine/src/libs/shader/exec/fShader.glsl");
    myShader.use();
    myShader.setInt("texture1", 0);
    myShader.setInt("texture2", 1);

    sf::Clock clock;
    while (window.isOpen()) {
        /* Update */
        userInput(window);
        float time = clock.getElapsedTime().asSeconds();
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
        projection = Mat4x4::perspective(radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
        projection *= Mat4x4::lookAt(Vec3(0), Vec3(0, 0, -1), Vec3(0, 1, 0));
        myShader.setMat4x4("projection", projection);

        // View
        view = Mat4x4(1.0f);
        view = view.translate(Vec3(0.0f, 0.0f, -5.0f));
        myShader.setMat4x4("view", view);

        // Model
        model = Mat4x4(1.0f);
        model = model.Scale(Vec3(1.2f));
        model = model.rotate(radians(-55.0f), Vec3(1.0f, 0.0f, 0.0f));
        model = model.translate(Vec3(myPos));
        myShader.setMat4x4("model", model);


        /* Render */
        glClearColor(0.7f, 0.7f, 7.0f, 0.0f); // 0.0f - 1.0f
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // hehe

        // One shape
        myShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_head_1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_head_2);
        VAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // two Shape
        model = Mat4x4(1.0f);
        model = model.Scale(Vec3(0.5f));
        model = model.translate(Vec3(-0.5f, -0.2f, 0.0f));
        model = model.rotate(radians(-55.0f) * time, Vec3(xValue, yValue, 0.5f));
        myShader.setMat4x4("model", model);
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
    return 0;
}

void onResize(const sf::Event &event) {
    glViewport(0, 0, event.size.width, event.size.height);
}

void userInput(sf::Window &window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        myPos.y += 0.005f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        myPos.y -= 0.005f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        myPos.x += 0.005f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        myPos.x -= 0.005f;
}

void mouseCursorPosition(const sf::Event &event) {
    std::cout << "Position: " << event.mouseMove.x << " " << event.mouseMove.y << std::endl;
}

void mouseScrollCallback(const sf::Event &event) {
    std::cout << "Scroll: " << event.mouseWheelScroll.delta << std::endl;
}

unsigned int loadTexture(const char *texture_path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texture_path, &width, &height, &nrChannels, 0);

    if (data) {
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