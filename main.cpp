#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <cmath>
#include <shader.h>
#include <vbuffer.h>
#include <varray.h>

using namespace std;

// pos and color
float vertexRectangle[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 4;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(1200, 800), "Hello World");
    window.setActive(true);

    // Check Glew
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cout << "Error:: glew not init =(: " << glewGetErrorString(glewError) << std::endl;
        return -1;
    }

    /* Shader */
    Shader myShader("C:/Users/motattack/CLionProjects/CG_Engine/src/libs/shader/exec/vShader.glsl",
                    "C:/Users/motattack/CLionProjects/CG_Engine/src/libs/shader/exec/fShader.glsl");

    /* Buffers */
    //unsigned int VAO;

    vBuffer VBO(vertexRectangle, sizeof(vertexRectangle) / sizeof(float));
    vArray VAO;
    VAO.attrPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexRectangle), &vertexRectangle, GL_STATIC_DRAW);

    /* Position Attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    /* Color Attribute*/
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    sf::Clock clock;
    while (window.isOpen()) {
        /* Update */
        float time = clock.getElapsedTime().asSeconds();
        float xValue = std::cos(time) / 2.0f + 0.5f; // 0.0f - 1.0f

        myShader.setFloat("x_color", xValue);

        /* Render */
        glClearColor(0.7f, 0.7f, 7.0f, 0.0f); // 0.0f - 1.0f
        glClear(GL_COLOR_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // hehe

        myShader.use();
        VAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Display
        window.display();

        // Poll events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}