#include <CG_Engine/engine.h>
#include <CG_Engine/ui/gui.h>
#include <CG_Engine/components/camera.h>
#include <CG_Engine/ui/input.h>
#include <CG_Engine/events.h>
#include <CG_Engine/base/Entity.h>
#include "CG_Engine/components/transform.h"
#include "CG_Engine/systems/common.h"
#include "CG_Engine/source.h"


void Engine::update() {
    event.poll();
    timer.tick();
    sf::Time dt = sf::seconds(timer.deltaTime());

    Manager.update();
    gui.display();

    window.display();
}

void Engine::init() {
    Resource.init();

    Manager.addSystem<CameraSystem>();
    EntityId cameraEntity = Manager.addNewEntity();
    Manager.addComponent<Camera>(cameraEntity); // Example camera position and rotation
    Manager.addComponent<Transform>(cameraEntity);

    Manager.start();

    gui.init();
}

Engine::Engine() : isRun(true), isGame(false), viewSize(0) {
    viewSize = Vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
    window.create(sf::VideoMode(viewSize.x, viewSize.y), "CG_Engine",
                  sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize,
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

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

Engine::~Engine() {
    window.close();
}

void Engine::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::startGame() {
    Manager.deactivateEditorSystems();
    Manager.activateRuntimeSystems();
    isGame = true;
};


void Engine::stopGame() {
    isGame = false;
    Manager.deactivateRuntimeSystems();
    Manager.activateEditorSystems();
}