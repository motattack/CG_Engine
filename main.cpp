#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <camera.h>
#include <model.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#define SCR_WIDTH 1200
#define SCR_HEIGHT 800

// pos and color and TexCoords
float vertex[] = {
        /* Top Position */        /* Color */            /* TexCoords */        /* Normals */
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        /* Bottom Position */        /* Color */            /* TexCoords */        /* Normals */
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        /* Left Position */        /* Color */            /* TexCoords */            /* Normals */
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        /* Right Position */        /* Color */            /* TexCoords */         /* Normals */
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        /* Back Position */        /* Color */            /* TexCoords */        /* Normals */
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        /* Front Position */        /* Color */            /* TexCoords */        /* Normals */
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,

};

void onResize(const sf::Event &event); // Protype
void userInput(sf::Window &window);

void mouseCursorPosition(const sf::Event &event); // Protype
void mouseScrollCallback(const sf::Event &event);

// Matrix's
Mat4x4 projection;
Mat4x4 model;
Mat4x4 view;

Vec3 lightPos = Vec3(1.2f, 1.0f, 1.5f);

// Camera
Camera camera(Vec3(0.0f, 0.0f, 3.0f));
float lastX = float(SCR_WIDTH) / 2.0f;
float lastY = float(SCR_HEIGHT) / 2.0f;
bool isFirstMouse = true;


// Frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    settings.attributeFlags = sf::ContextSettings::Default;

    sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT, 32), "First Window",
                            sf::Style::Titlebar | sf::Style::Close, settings);
    window.setVisible(true);
    window.setFramerateLimit(75);

    ImGui::SFML::Init(window, true);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Error: glew not init =(" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // Cube buffers
    vArray VAO;
    vBuffer VBO(vertex, sizeof(vertex));

    vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) nullptr);

    vArray::attrPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (3 * sizeof(float)));

    vArray::attrPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (6 * sizeof(float)));

    // Normals Attribute
    vArray::attrPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (8 * sizeof(float)));

    /* Light Buffers */
    vArray lightCubeVAO;
    vBuffer lightCubeVBO(vertex, sizeof(vertex));

    /* Light Position Attribute */
    vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) nullptr);

    stbi_set_flip_vertically_on_load(true);

    /* Model */
    Model myBackPack("../res/Object/Backpack/backpack.obj");

    /* Shader */
    Shader myShader("../res/shader/vShader.glsl",
                    "../res/shader/fShader.glsl");
    myShader.use();
    Shader lightCubeShader("../res/Shader/lCube.vert", "../res/Shader/lCube.frag");


    sf::Clock deltaClock, clock;
    while (window.isOpen()) {
        /* Update */
        userInput(window);
        float time = clock.getElapsedTime().asSeconds();

        deltaTime = time - lastFrame;
        lastFrame = time;

        float radius = 5.0f;
        float camX = std::sin(time) * radius;
        float camZ = std::cos(time) * radius;

        // Gui Variables
        glClearColor(0.7f, 0.7f, 7.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // First Object
        myShader.use();

        lightPos = Vec3(camX, camZ, 0.0f);
        myShader.setVec3("light.position", camera.Position);
        myShader.setVec3("light.direction", camera.Front);
        myShader.setVec3("light.ambient", Vec3(0.2f));
        myShader.setVec3("light.diffuse", Vec3(0.4f));
        myShader.setVec3("light.specular", Vec3(0.5f));
        myShader.setVec3("viewPos", camera.Position);

        /* Coordinates */
        // Projection
        projection = Mat4x4::perspective(radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
        myShader.setMat4x4("projection", projection);

        // View
        view = Mat4x4(1.0f);
        view = camera.GetViewMatrix();
        myShader.setMat4x4("view", view);

        // Model
        model = Mat4x4(1.0f);
        myShader.setMat4x4("model", model);

        // Draw your model
        myBackPack.Draw(myShader);

        // Second Object
        lightCubeShader.use();
        lightCubeShader.setMat4x4("projection", projection);
        lightCubeShader.setMat4x4("view", view);
        model = Mat4x4(1.0f);
        model = model.translate(lightPos);
        lightCubeShader.setMat4x4("model", model);

        lightCubeVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        ImGui::SFML::Update(window, deltaClock.restart());

        glEnable(GL_DEPTH_TEST);

        float FPS = ImGui::GetIO().Framerate;
        ImGui::Begin("Hello, world!");
        ImGui::Text("FPS = %f", FPS);
        ImGui::End();

        ImGui::SFML::Render(window);

        glEnable(GL_DEPTH_TEST);

        window.display();

        // Poll events
        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
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

    ImGui::SFML::Shutdown();
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
        camera.ProcessKeyboard(FORWARD, deltaTime);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        camera.ProcessKeyboard(RIGHT, deltaTime);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        camera.ProcessKeyboard(UP, deltaTime);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        camera.ProcessKeyboard(DOWN, deltaTime);
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouseScrollCallback(const sf::Event &event) {
    camera.ProcessMouseScroll(event.mouseWheelScroll.delta);
}