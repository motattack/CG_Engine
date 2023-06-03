#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(500, 500, 32), "GL_Engine",
                      sf::Style::Titlebar | sf::Style::Close, settings);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Error:: glew not init =(" << std::endl;
        return -1;
    }

    const int verts = 6;

    float polygon[verts * (3 + 4)] = {
            /*        position           */   /*             color              */
            -0.5F, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.0f, -0.75f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,
            -0.5, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts * (3 + 4), polygon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    const char *shaderTextVertex =
            "#version 330 core\n"
            "\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec4 aColor;\n"
            "\n"
            "out vec4 fragColor;\n"
            "\n"
            "void main() {\n"
            "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
            "    fragColor = aColor;\n"
            "}";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderTextVertex, NULL);
    glCompileShader(vertexShader);

    char resultInfo[1000];
    int res;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &res);
    if (res == 0) {
        glGetShaderInfoLog(vertexShader, 1000, NULL, resultInfo);
        std::cout << "Shader compile error: " << resultInfo << std::endl;
        return -1;
    }

    const char *shaderTextFragment =
            "#version 330 core\n"
            "\n"
            "in vec4 fragColor;\n"
            "out vec4 outColor;\n"
            "\n"
            "void main(){\n"
            "    outColor = fragColor;\n"
            "}";

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (char **const) (&shaderTextFragment), NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &res);
    if (res == 0) {
        glGetShaderInfoLog(fragmentShader, 1000, NULL, resultInfo);
        std::cout << "Shader compile error: " << resultInfo << std::endl;
        return -1;
    }

    unsigned int shaderProg;
    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader);
    glLinkProgram(shaderProg);

    glGetProgramiv(shaderProg, GL_LINK_STATUS, &res);
    if (res == 0) {
        glGetProgramInfoLog(shaderProg, 1000, NULL, resultInfo);
        std::cout << "Shader linking error: " << resultInfo << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, verts);

        window.display();
    }

    window.close();
    return 0;
}