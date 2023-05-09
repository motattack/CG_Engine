#ifndef CG_ENGINE_SCREEN_H
#define CG_ENGINE_SCREEN_H

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Screen {
public:
    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;


    void framebufferSizeCallback(sf::RenderWindow &window, int width, int height) {
        glViewport(0, 0, width, height);

        SCR_WIDTH = width;
        SCR_HEIGHT = height;
    }

    Screen()
            : window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT, 32), "First Window",
                                 sf::Style::Titlebar | sf::Style::Close, settings) {};

    bool init() {
        window.setFramerateLimit(75);
        window.setVerticalSyncEnabled(true);
        window.setActive(true);

        ImGui::SFML::Init(window, true);


        // Enable docking
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Set context settings properties
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.majorVersion = 4;
        settings.minorVersion = 6;
        settings.attributeFlags = sf::ContextSettings::Default;

        window.create(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "First Window", sf::Style::Titlebar | sf::Style::Close, settings);
        return true;
    };

    void setParameters() const {
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        glEnable(GL_DEPTH_TEST);
    };

    // main loop
    static void update() {
        glClearColor(0.7f, 0.7f, 7.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

    sf::RenderWindow window;
    sf::ContextSettings settings;
};

#endif //CG_ENGINE_SCREEN_H
