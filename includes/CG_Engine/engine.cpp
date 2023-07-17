#include <CG_Engine/engine.h>
#include <CG_Engine/ui/gui.h>
#include <CG_Engine/components/camera.h>
#include <CG_Engine/base/Entity.h>
#include <CG_Engine/systems/common.h>


void Engine::update() {
    Manager.update();
    Manager.render();
    glEnable(GL_DEPTH_TEST);

    window.display();
}

void Engine::init() {
    // register component list
    Manager.registerCompList<Camera>();
    Manager.registerCompList<Transform>();
    Manager.registerCompList<SpotLighting>();
    Manager.registerCompList<EntityName>();
    Manager.registerCompList<PointLighting>();
    Manager.registerCompList<MeshRenderer>();
    Manager.registerCompList<ModelRenderer>();
    Manager.registerCompList<DirectionalLight>();

    // register component factory
    Registrar<Camera>("Camera");
    Registrar<SpotLighting>("Spot Light");
    Registrar<SpotLighting>("Point Light");
    Registrar<MeshRenderer>("MeshRenderer");
    Registrar<ModelRenderer>("ModelRenderer");
    Registrar<DirectionalLight>("Directional Light");
    Registrar<EntityName>("EntityName");

    // register systems
    Manager.addSystem<SpotLightSystem>();
    Manager.addSystem<PointLightSystem>();
    Manager.addSystem<DirectionalLightSystem>();

    Manager.addSystem<MeshRendererSystem>();
    Manager.addSystem<ModelRendererSystem>();

    // runtime systems will be added at runtime
    Manager.addRuntimeSystem<CameraSystem>();

    Manager.activateEditorSystems();
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

    if (!isGame) {
//        gui.display();
    } else {
        Manager.render();
    }
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