#include <CG_Engine/engine.h>
#include <CG_Engine/ui/gui.h>
#include <CG_Engine/components/camera.h>
#include <CG_Engine/ui/input.h>
#include <CG_Engine/events.h>
#include <CG_Engine/base/Entity.h>
#include "CG_Engine/components/transform.h"
#include "CG_Engine/systems/common.h"
#include "CG_Engine/source.h"
#include "CG_Engine/components/name.h"
#include "CG_Engine/systems/meshSystem.h"
#include "CG_Engine/systems/modelSystem.h"
#include "CG_Engine/systems/directionalLightSystem.h"
#include "CG_Engine/systems/pointLightSystem.h"
#include "CG_Engine/systems/spotLightSystem.h"


void Engine::update() {
    event.poll();
    timer.tick();
    sf::Time dt = sf::seconds(timer.deltaTime());

    Manager.update();
    Manager.render();
    gui.display();
    glEnable(GL_DEPTH_TEST);

    window.display();
}

void Engine::init() {
    Resource.init();

    Manager.registerCompList<Camera>();
    Manager.registerCompList<Transform>();
    Manager.registerCompList<EntityName>();
    Manager.registerCompList<MeshRenderer>();

    Registrar<Camera>("Camera");
    Registrar<MeshRenderer>("MeshRenderer");

    Manager.addSystem<SpotLightSystem>();
    EntityId spotEntity = Manager.addNewEntity();
    Manager.addComponent<SpotLighting>(spotEntity);
    Manager.addComponent<Transform>(spotEntity);
    Manager.addComponent<EntityName>(spotEntity, "Spot 1");

    Manager.addSystem<PointLightSystem>();
    EntityId pointEntity = Manager.addNewEntity();
    Manager.addComponent<PointLighting>(pointEntity);
    Manager.addComponent<Transform>(pointEntity);
    Manager.addComponent<EntityName>(pointEntity, "Point 1");

    Manager.addSystem<DirectionalLightSystem>();
    EntityId dirEntity = Manager.addNewEntity();
    Manager.addComponent<DirectionalLight>(dirEntity);
    Manager.addComponent<Transform>(dirEntity);
    Manager.addComponent<EntityName>(dirEntity, "Dir 1");

    Manager.addSystem<ModelRendererSystem>();
    EntityId modelEntity = Manager.addNewEntity();
    Manager.addComponent<ModelRenderer>(modelEntity, "PACK");
    Manager.addComponent<Transform>(modelEntity);
    Manager.addComponent<EntityName>(modelEntity, "PACK 1");

    Manager.addSystem<MeshRendererSystem>();
    EntityId meshEntity = Manager.addNewEntity();
    Manager.addComponent<MeshRenderer>(meshEntity, "CUBE");
    Manager.addComponent<Transform>(meshEntity);
    Manager.addComponent<EntityName>(meshEntity, "CUBE 1");
    EntityId meshEntity2 = Manager.addNewEntity();
    Manager.addComponent<MeshRenderer>(meshEntity2, "SPHERE");
    Manager.addComponent<Transform>(meshEntity2);
    Manager.addComponent<EntityName>(meshEntity2, "SPHERE 2");

    Manager.addSystem<CameraSystem>();
    EntityId cameraEntity = Manager.addNewEntity();
    Manager.addComponent<Camera>(cameraEntity);
    Manager.addComponent<Transform>(cameraEntity);
    Manager.addComponent<EntityName>(cameraEntity, "MainCamera");

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