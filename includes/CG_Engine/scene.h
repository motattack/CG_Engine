#ifndef CG_ENGINE_SCENE_H
#define CG_ENGINE_SCENE_H

#include <vector>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/ui/camera.h>
#include <CG_Engine/math/vec3.h>
#include <CG_Engine/render/light.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <CG_Engine/ui/mouse.h>
#include <CG_Engine/ui/keyboard.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

class Scene {
public:
    Scene() = default;

    Scene(const char *title, unsigned int scrWidth, unsigned int scrHeight) : title(title) {
        this->scrWidth = scrWidth;
        this->scrHeight = scrHeight;

        setWindowColor(0.1f, 0.15f, 0.15f, 1.0f);
    };


    void setWindowColor(float r, float g, float b, float a) {
        bg[0] = r;
        bg[1] = g;
        bg[2] = b;
        bg[3] = a;
    };

    bool init() {
        window.create(sf::VideoMode(scrWidth, scrHeight), title, sf::Style::Titlebar | sf::Style::Close,
                      settings);

        window.setFramerateLimit(75);
        window.setVerticalSyncEnabled(true);
        window.setActive(true);

        ImGui::SFML::Init(window, true);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        glewExperimental = GL_TRUE;

        if (GLEW_OK != glewInit()) {
            std::cout << "Error:: glew not init =(" << std::endl;
            return -1;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        setPolygonMode();

        stbi_set_flip_vertically_on_load(true);

        return true;
    };

    bool shouldClose() {
        return !window.isOpen(); // Return true if the SFML window is closed
    }

    void setShouldClose() {
        window.close();
    };

    void newFrame() {
        window.display();
    }

    void cleanup() {
        ImGui::SFML::Shutdown();
        window.close();
    };

    bool pollEvent(sf::Event &event) {
        return window.pollEvent(event);
    }

    void setPolygonMode() const {
        if (wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void processInput(float dt) {
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

        if (Keyboard::keyWentUp(sf::Keyboard::L)) {
            torch = !torch;
        }

        camera.Rotate(Mouse::getDX(), Mouse::getDY());

        camera.ChangeFOV(Mouse::getScrollDY());

        if (Keyboard::key(sf::Keyboard::Escape)) {
            setShouldClose();
        }

        if (Keyboard::keyWentUp(sf::Keyboard::E)) {
            wireframeMode = !wireframeMode;
            setPolygonMode();
        }
    }

    bool getTorch() const {
        return torch;
    }

    void PollEvents() {
        sf::Event event{};
        while (pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            Keyboard::keyCallback(event);
            switch (event.type) {
                case sf::Event::Closed:
                    cleanup();
                    break;
                case sf::Event::MouseMoved:
                    Mouse::cursorPosCallback(static_cast<float>(event.mouseMove.x),
                                             static_cast<float>(event.mouseMove.y));
                    break;
                case sf::Event::MouseWheelScrolled:
                    Mouse::mouseWheelCallback(event.mouseWheelScroll.delta);
                    break;
                default:
                    break;
            }
        }
    }

    sf::RenderWindow window;
    Camera camera;
protected:

    sf::ContextSettings settings{24, 8, 0, 3, 3, sf::ContextSettings::Default};

    const char *title{};
    unsigned int scrWidth = 800;
    unsigned int scrHeight = 600;
    bool torch = false;
    bool wireframeMode = false;

    float bg[4]{}; // background color
};

#endif //CG_ENGINE_SCENE_H
