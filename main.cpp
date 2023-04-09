#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>


using namespace std;

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 4;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(800, 600, 32), "Welcome",
                      sf::Style::Titlebar | sf::Style::Close);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        cout << "Error:: glew not init =(" << endl;
        return -1;
    }

    const char *vshader = "#version 330 core\n"
                          "layout (location = 0) in vec3 inPos;\n"
                          "void main() {\n"
                          "gl_Position = vec4(inPos, 1.0);"
                          "}\n";
    const char *fshader = "#version 330 core\n"
                          "out vec4 OutColor;\n"
                          "void main() {\n"
                          "OutColor = vec4(0.38f, 0.01f, 0.37f, 0.0f);\n"
                          "}\n";

    int vshaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshaderId, 1, &vshader, nullptr);
    glCompileShader(vshaderId);

    int result = 0;
    glGetShaderiv(vshaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        char log[512];
        glGetShaderInfoLog(vshaderId, 512, nullptr, log);
    }

    int fshaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshaderId, 1, &fshader, nullptr);
    glCompileShader(fshaderId);

    glGetShaderiv(fshaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        char log[512];
        glGetShaderInfoLog(fshaderId, 512, nullptr, log);
    }

    int shaderId = glCreateProgram();
    glAttachShader(shaderId, vshaderId);
    glAttachShader(shaderId, fshaderId);
    glLinkProgram(shaderId);

    glGetShaderiv(shaderId, GL_LINK_STATUS, &result);
    if (!result) {
        char log[512];
        glGetShaderInfoLog(shaderId, 512, nullptr, log);
    }

    glDeleteShader(vshaderId);
    glDeleteShader(fshaderId);

    float verts[]{
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
    };

    unsigned index[] = {
            0, 1, 2
    };

    unsigned VA0 = 0, VBA = 0, IBA = 0;
    glCreateVertexArrays(1, &VA0);
    glGenBuffers(1, &VBA);
    glGenBuffers(1, &IBA);

    glBindVertexArray(VA0);

    glBindBuffer(GL_ARRAY_BUFFER, VBA);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBA);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


    bool isGo = true;

    while (isGo) {
        sf::Event windowEvent{};

        while (window.pollEvent(windowEvent)) {
            switch (windowEvent.type) {
                case sf::Event::Closed:
                    isGo = false;
                    break;
                default:
                    break;
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderId);
        glBindVertexArray(VA0);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        window.display();
    }

    window.close();
    return 0;
}