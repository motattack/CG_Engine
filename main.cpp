#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <camera.h>
#include <shader.h>
#include <vbuffer.h>
#include <varray.h>

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

Vec3 pointLightPositions[] = {
        Vec3(0.7f, 0.2f, 2.0f),
        Vec3(2.3f, -3.3f, -4.0f),
        Vec3(-4.0f, 2.0f, -12.0f),
        Vec3(0.0f, 0.0f, -3.0f)
};

Vec3 cubePositions[] =
        {
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

    vBuffer VBO(vertex, sizeof(vertex));
    vArray VAO;

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

    /* Texture */
    stbi_set_flip_vertically_on_load(true);
    GLuint container_texture = loadTexture(
            "../res/texture/example/head.png");
    GLuint face_texture = loadTexture(
            "../res/texture/example/head2.png");

    GLuint diffuseMap = loadTexture("../res/texture/example/container2.png");
    GLuint specularMap = loadTexture("../res/texture/example/container2_specular.png");

    /* Shader */
    Shader myShader("../res/shader/vShader.glsl",
                    "../res/shader/fShader.glsl");
    Shader lightCubeShader("../res/Shader/lCube.vert",
                           "../res/Shader/lCube.frag");
    myShader.use();
    myShader.setInt("material.diffuse", 0);
    myShader.setInt("material.specular", 1);

    sf::Clock deltaClock, clock;
    while (window.isOpen()) {
        /* Update */
        userInput(window);
        float time = clock.getElapsedTime().asSeconds();

        deltaTime = time - lastFrame;
        lastFrame = time;
        float radius = 3.0f;
        float camX = std::sin(time) * radius;
        float camZ = std::cos(time) * radius;

        // Gui Variables
        glClearColor(0.7f, 0.7f, 7.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // First Object
        myShader.use();

        //  Flashlights
        myShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.5f);
        myShader.setVec3("dirLight.ambient", Vec3(0.2f));
        myShader.setVec3("dirLight.diffuse", Vec3(0.4f));
        myShader.setVec3("dirLight.specular", Vec3(0.5f));

        for (int i = 0; i < 4; i++) {
            myShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            myShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", Vec3(0.2f));
            myShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", Vec3(0.4f));
            myShader.setVec3("pointLights[" + std::to_string(i) + "].specular", Vec3(0.9f));
            myShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            myShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
            myShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
        }

        // Spot Lights
        myShader.setVec3("spotLight.position", camera.Position);
        myShader.setVec3("spotLight.direction", camera.Front);
        myShader.setVec3("viewPos", camera.Position);
        myShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        myShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
        myShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0);
        myShader.setFloat("spotLight.constant", 1.0f);
        myShader.setFloat("spotLight.linear", 0.09f);
        myShader.setFloat("spotLight.quadratic", 0.032f);
        myShader.setFloat("spotLight.cutOff", cos(radians(13.0f)));
        myShader.setFloat("spotLight.outerCutOff", cos(radians(15.5f)));

        // Material
        myShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        myShader.setFloat("material.shininess", 64.0f);

        /* Coordinates */
        // Projection
        projection = Mat4x4::perspective(radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
        myShader.setMat4x4("projection", projection);

        // View
        view = Mat4x4(1.0f);
        view = camera.GetViewMatrix();
        myShader.setMat4x4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        for (auto cubePosition: cubePositions) {
            // Model
            model = Mat4x4(1.0f);
            model = model.translate(Vec3(cubePosition));
            model = model.rotate(radians(-55.0f) * time, Vec3(0.0f, 1.0f, 0.0f));

            myShader.setMat4x4("model", model);
            VAO.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        vArray::unbind();

        // Second Object
        lightCubeShader.use();
        lightCubeShader.setMat4x4("projection", projection);
        lightCubeShader.setMat4x4("view", view);


        for (auto pointLightPosition : pointLightPositions) {
            // Model
            model = Mat4x4(1.0f);
            model = model.translate(pointLightPosition);
            lightCubeShader.setMat4x4("model", model);
            lightCubeVAO.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        vArray::unbind();

        // Second Object
        lightCubeShader.use();
        lightCubeShader.setMat4x4("projection", projection);
        lightCubeShader.setMat4x4("view", view);
        // Model
        model = Mat4x4(1.0f);
        model = model.translate(lightPos);
        model = model.Scale(Vec3(0.3f));
        lightCubeShader.setMat4x4("model", model);
        lightCubeVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        vArray::unbind();

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