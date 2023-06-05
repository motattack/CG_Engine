#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <CG_Engine/ui/camera.h>
#include <CG_Engine/ui/keyboard.h>
#include <CG_Engine/ui/mouse.h>
#include <CG_Engine/objects/model.h>
#include <CG_Engine/vertex/memory.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include <imgui.h>
#include <imgui-SFML.h>

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

    if (Keyboard::key(sf::Keyboard::Left)) {
        camera.Rotate(-dt * 360.f, 0);
    }

    if (Keyboard::key(sf::Keyboard::Right)) {
        camera.Rotate(dt * 360.f, 0);
    }

    if (Keyboard::key(sf::Keyboard::Up)) {
        camera.Rotate(0, dt * 360.f);
    }

    if (Keyboard::key(sf::Keyboard::Down)) {
        camera.Rotate(0, -dt * 360.f);
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
    settings.attributeFlags = sf::ContextSettings::Default;

    sf::RenderWindow window(sf::VideoMode(1280, 720, 32), "GL_Engine",
                            sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(75);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    ImGui::SFML::Init(window, true);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Error:: glew not init =(" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    setPolygonMode();
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    float cube[] = {
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

    Vec3 lightPos = Vec3(1.2f, 1.0f, 1.5f);

    vArray VAO;
    vBuffer VBO(cube, sizeof(cube));

    vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) nullptr);

    vArray::attrPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (3 * sizeof(float)));

    vArray::attrPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (6 * sizeof(float)));

    // Normals Attribute
    vArray::attrPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (8 * sizeof(float)));

    /* Light Buffers */
    vArray lightCubeVAO;
    vBuffer lightCubeVBO(cube, sizeof(cube));

    /* Light Position Attribute */
    vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) nullptr);

    stbi_set_flip_vertically_on_load(true);
    GLuint diffuseMap = TextureFromFile("container2.png", "res/textures/example");
    GLuint specularMap = TextureFromFile("container2_specular.png", "res/textures/example");

    /* Shader */
    Shader myShader("res/shader/vShader.glsl",
                    "res/shader/fShader.glsl");
    Shader lightCubeShader("res/Shader/lCube.vert", "res/Shader/lCube.frag");
    //myShader.use();


    Model myBackPack("res/models/backpack/backpack.obj");

    float dt;
    float lastFrame = 0.0f;

    sf::Clock clock, deltaClock;
    while (window.isOpen()) {
        float currentTime = clock.getElapsedTime().asSeconds();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window, dt);

        float radius = 3.0f;
        float camX = std::sin(currentTime) * radius;
        float camZ = std::cos(currentTime) * radius;
        float xValue = std::cos(currentTime) / 2.0f + 0.5f; // 0.0f - 1.0f

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

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // First Object
        myShader.use();

        // Directional Light
        myShader.set3Float("dirLight.direction", Vec3(dir_direction[0], dir_direction[1], dir_direction[2]));
        myShader.set3Float("dirLight.ambient", Vec3(dir_ambient[0], dir_ambient[1], dir_ambient[2]));
        myShader.set3Float("dirLight.diffuse", Vec3(dir_diffuse[0], dir_ambient[1], dir_ambient[2]));
        myShader.set3Float("dirLight.specular", Vec3(dir_specular[0], dir_ambient[1], dir_ambient[3]));

        // Point Lights
        for (int i = 0; i < 4; i++) {
            myShader.set3Float(("pointLights[" + std::to_string(i) + "].position").c_str(),
                               pointLightPositions[i] * xValue);
            myShader.set3Float(("pointLights[" + std::to_string(i) + "].ambient").c_str(),
                               Vec3(point_ambient[0], point_ambient[1], point_ambient[2]));
            myShader.set3Float(("pointLights[" + std::to_string(i) + "].diffuse").c_str(),
                               Vec3(point_diffuse[0], point_diffuse[1], point_diffuse[2]));
            myShader.set3Float(("pointLights[" + std::to_string(i) + "].specular").c_str(),
                               Vec3(point_specular[0], point_specular[1], point_specular[2]));
            myShader.setFloat(("pointLights[" + std::to_string(i) + "].constant").c_str(), point_constant);
            myShader.setFloat(("pointLights[" + std::to_string(i) + "].linear").c_str(), point_linear);
            myShader.setFloat(("pointLights[" + std::to_string(i) + "].quadratic").c_str(), point_quadratic);
        }

        // Spot Light
        myShader.set3Float("spotLight.position", camera.Position);
        myShader.set3Float("spotLight.direction", camera.Front);
        myShader.set3Float("viewPos", camera.Position);
        myShader.set3Float("spotLight.ambient", Vec3(spot_ambient[0], spot_ambient[1], spot_ambient[2]));
        myShader.set3Float("spotLight.diffuse", Vec3(spot_diffuse[0], spot_diffuse[1], spot_diffuse[2]));
        myShader.set3Float("spotLight.specular", Vec3(spot_specular[0], spot_specular[1], spot_specular[2]));
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
        projection = camera.GetProjectionMatrix();
        myShader.setMat4("projection", projection);

        // View
        view = camera.GetViewMatrix();
        myShader.setMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // We Draw a map with three loop cuz we are rendering in 3D so each axies need a loop like (x -> i, y -> j, z -> k).
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                for (int k = 0; k < 10; k++) {
                    // We will set blocks like minecraft and we wannna make it to a hollow huge Maps with 10x10x10 cubes
                    if (i == 0 || i == 9 || j == 0 || j == 9 || k == 9) {
                        // Model
                        model = Mat4x4(1.0f);
                        model = model.translate(Vec3(i + cubeMapPos[0], j + cubeMapPos[1], k + cubeMapPos[2]));
                        //model = model.rotate(radians(-55.0f) * currentTime, Vec3(0.0f, 1.0f, 0.0f));

                        myShader.setMat4("model", model);
                        VAO.bind();
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }
            }
        }

        model = Mat4x4(1.0f);
        myShader.setMat4("model", model);

        myBackPack.Draw(myShader);

//
        // Second Object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);


        for (auto pointLightPosition: pointLightPositions) {
            // Model
            model = Mat4x4(1.0f);
            model = model.translate(pointLightPosition * xValue);
            model = model.scale(Vec3(0.3f));
            lightCubeShader.setMat4("model", model);
            lightCubeVAO.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Second Object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        ImGui::SFML::Update(window, deltaClock.restart());

        glEnable(GL_DEPTH_TEST);

        float FPS = ImGui::GetIO().Framerate;
        ImGui::Begin("Hello, world!");
        ImGui::Text("FPS = %f", FPS);

        ImGuiStyle *style = &ImGui::GetStyle();
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


        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
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