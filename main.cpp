#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <cmath>

using namespace std;

// pos and color
float vertexRectangle[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

const char *vertexShaderData =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 curColor;\n"
        "void main()\n"
        "{\n"
        "	curColor = aColor;\n"
        "	gl_Position = vec4(aPos, 1.0f);\n"
        "}\0";

const char *fragmentShaderData =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 curColor;\n"
        "uniform float x_color;\n"
        "void main()\n"
        "{\n"
        "	FragColor = vec4(x_color * curColor.x, curColor.y, curColor.z ,1.0f);\n"
        "}\0";

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
    // vertex
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex, 1, &vertexShaderData, NULL);
    glCompileShader(vertex);

    int success;
    char infolog[512];

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infolog);
        std::cout << "Error shader vertex: " << infolog << std::endl;
    }
    //fragment
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment, 1, &fragmentShaderData, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infolog);
        std::cout << "Error shader fragment:" << infolog << std::endl;
    }

    //Program
    GLuint program = glCreateProgram();

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infolog);
        std::cout << "Error shader program: " << infolog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    /* Buffers */
    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexRectangle), &vertexRectangle, GL_STATIC_DRAW);

    /* Position Attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Color Attribute*/
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    sf::Clock clock;
    while (window.isOpen()) {
        /* Update */
        float time = clock.getElapsedTime().asSeconds();
        float xValue = std::cos(time) / 2.0f + 0.5f; // 0.0f - 1.0f

        glUniform1f(glGetUniformLocation(program, "x_color"), xValue);

        /* Render */
        glClearColor(0.7f, 0.7f, 7.0f, 0.0f); // 0.0f - 1.0f
        glClear(GL_COLOR_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // hehe

        glUseProgram(program);
        glBindVertexArray(VAO);
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