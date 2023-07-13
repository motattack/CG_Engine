#include "engine.h"
#include <CG_Engine/systems/common.h>
#include <CG_Engine/ui/gui.h>

void Engine::initEntity() {
    Manager.registerSystem<CameraSystem>();
}

void Engine::update() {
    timer.tick();
    sf::Time dt = sf::seconds(timer.deltaTime());

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gui.display(window);

    window.display();
}

void Engine::init() {
    timer.init();
    gui.init();
    initEntity();
}

Engine::Engine() : isRun(true), width(SCREEN_WIDTH), height(SCREEN_HEIGHT) {
    window.create(sf::VideoMode(width, height), "CG_Engine", sf::Style::Titlebar | sf::Style::Close,
                  settings);
    window.setFramerateLimit(75);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    ImGui::SFML::Init(window, true);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        throw std::runtime_error("Error:: glew not init =(");
    }

    glEnable(GL_DEPTH_TEST);
};
