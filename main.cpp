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
        float xValue = std::cos(time) / 2.0f + 0.5f; // 0.0f - 1.0f

        // Gui Variables
        // Directional Light
        static float dir_direction[3] = {-0.2f, -1.0f, -0.5f};
        static float dir_ambient[3] = {0.2f, 0.2f, 0.2f};
        static float dir_diffuse[3] = {0.4f, 0.4f, 0.4f};
        static float dir_specular[3] = {0.6f, 0.6f, 0.6f};

        // Point Light
        //static float point_position
        static float point_ambient[3] = {0.2f, 0.2f, 0.2f};
        static float point_diffuse[3] = {0.5f, 0.5f, 0.5f};
        static float point_specular[3] = {1.0f, 1.0f, 1.0f};
        static float point_constant = 1.0f;
        static float point_linear = 0.09f;
        static float point_quadratic = 0.032f;

        // Spot Light
        static float spot_ambient[3] = {0.2f, 0.2f, 0.2f};
        static float spot_diffuse[3] = {1.0f, 1.0f, 1.0f};
        static float spot_specular[3] = {1.0f, 1.0f, 1.0f};
        static float spot_constant = 1.0f;
        static float spot_linear = 0.09f;
        static float spot_quadratic = 0.032f;
        static float spot_cutOff = 12.5f;
        static float spot_outerCutOff = 18.5f;

        static float cubeMapPos[3] = {0.0f, 0.0f, 0.0f};

        // Gui Variables
        glClearColor(0.7f, 0.7f, 7.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // First Object
        myShader.use();

        // Directional Light
        myShader.setVec3("dirLight.direction", Vec3(dir_direction[0], dir_direction[1], dir_direction[2]));
        myShader.setVec3("dirLight.ambient", Vec3(dir_ambient[0], dir_ambient[1], dir_ambient[2]));
        myShader.setVec3("dirLight.diffuse", Vec3(dir_diffuse[0], dir_ambient[1], dir_ambient[2]));
        myShader.setVec3("dirLight.specular", Vec3(dir_specular[0], dir_ambient[1], dir_ambient[3]));

        // Point Lights
        for (int i = 0; i < 4; i++) {
            myShader.setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(),
                             pointLightPositions[i] * xValue);
            myShader.setVec3(("pointLights[" + std::to_string(i) + "].ambient").c_str(),
                             Vec3(point_ambient[0], point_ambient[1], point_ambient[2]));
            myShader.setVec3(("pointLights[" + std::to_string(i) + "].diffuse").c_str(),
                             Vec3(point_diffuse[0], point_diffuse[1], point_diffuse[2]));
            myShader.setVec3(("pointLights[" + std::to_string(i) + "].specular").c_str(),
                             Vec3(point_specular[0], point_specular[1], point_specular[2]));
            myShader.setFloat(("pointLights[" + std::to_string(i) + "].constant").c_str(), point_constant);
            myShader.setFloat(("pointLights[" + std::to_string(i) + "].linear").c_str(), point_linear);
            myShader.setFloat(("pointLights[" + std::to_string(i) + "].quadratic").c_str(), point_quadratic);
        }

        // Spot Light
        myShader.setVec3("spotLight.position", camera.Position);
        myShader.setVec3("spotLight.direction", camera.Front);
        myShader.setVec3("viewPos", camera.Position);
        myShader.setVec3("spotLight.ambient", Vec3(spot_ambient[0], spot_ambient[1], spot_ambient[2]));
        myShader.setVec3("spotLight.diffuse", Vec3(spot_diffuse[0], spot_diffuse[1], spot_diffuse[2]));
        myShader.setVec3("spotLight.specular", Vec3(spot_specular[0], spot_specular[1], spot_specular[2]));
        myShader.setFloat("spotLight.constant", spot_constant);
        myShader.setFloat("spotLight.linear", spot_linear);
        myShader.setFloat("spotLight.quadratic", spot_quadratic);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            myShader.setFloat("spotLight.cutOff", cos(radians(spot_cutOff)));
            myShader.setFloat("spotLight.outerCutOff", cos(radians(spot_outerCutOff)));
        } else {
            myShader.setFloat("spotLight.cutOff", cos(radians(0.0f)));
            myShader.setFloat("spotLight.outerCutOff", cos(radians(0.0f)));

        }

        // Material
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

        // We Draw a map with three loop cuz we are rendering in 3D so each axies need a loop like (x -> i, y -> j, z -> k).
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                for (int k = 0; k < 10; k++)
                {
                    // We will set blocks like minecraft and we wannna make it to a hollow huge Maps with 10x10x10 cubes
                    if (i == 0 || i == 9 || j == 0 || j == 9 || k == 9)
                    {
                        // Model
                        model = Mat4x4(1.0f);
                        model = model.translate(Vec3(i + cubeMapPos[0], j + cubeMapPos[1], k + cubeMapPos[2]));
                        //model = glm::rotate(model, glm::radians(-55.0f) * time, glm::vec3(0.0f, 1.0f, 0.0f));

                        myShader.setMat4x4("model", model);
                        VAO.bind();
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }
            }
        }
        vArray::unbind();

        // Second Object
        lightCubeShader.use();
        lightCubeShader.setMat4x4("projection", projection);
        lightCubeShader.setMat4x4("view", view);


        for (auto pointLightPosition: pointLightPositions) {
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

        ImGuiStyle* style = &ImGui::GetStyle();
        style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
        ImGui::PopStyleColor();

        ImGui::SliderFloat3("Map Position", cubeMapPos, -10.0f, 10.0f);
        ImGui::Text("Directional Light");
        ImGui::SliderFloat3("Directional Direction", dir_direction, -2.0f, 2.0f);
        ImGui::SliderFloat3("Directional Ambient", dir_ambient, 0.0f, 1.0f);
        ImGui::SliderFloat3("Directional Diffuse", dir_diffuse, 0.0f, 1.0f);
        ImGui::SliderFloat3("Directional Specular", dir_specular, 0.0f, 1.0f);
        ImGui::Text("Point Light");
        ImGui::SliderFloat3("Point Ambient", point_ambient, 0.0f, 1.0f);
        ImGui::SliderFloat3("Point Diffuse", point_diffuse, 0.0f, 1.0f);
        ImGui::SliderFloat3("Point Specular", point_specular, 0.0f, 1.0f);
        ImGui::SliderFloat("Point Constant", &point_constant, -1.0f, 1.0f);
        ImGui::SliderFloat("Point Linear", &point_linear, -1.0f, 1.0f);
        ImGui::SliderFloat("Point Quadratic", &point_quadratic, -1.0f, 1.0f);
        ImGui::Text("Spot Light");
        ImGui::SliderFloat3("Spot Ambient", spot_ambient, 0.0f, 1.0f);
        ImGui::SliderFloat3("Spot Diffuse", spot_diffuse, 0.0f, 1.0f);
        ImGui::SliderFloat3("Spot Specular", spot_specular, 0.0f, 1.0f);
        ImGui::SliderFloat("Spot Constant", &spot_constant, -1.0f, 1.0f);
        ImGui::SliderFloat("Spot Linear", &spot_linear, -1.0f, 1.0f);
        ImGui::SliderFloat("Spot Quadratic", &spot_quadratic, -1.0f, 1.0f);
        ImGui::SliderFloat("Spot CutOff", &spot_cutOff, 0.0f, 100.0f);
        ImGui::SliderFloat("Spot OuterCutOff", &spot_outerCutOff, 0.0f, 100.0f);
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