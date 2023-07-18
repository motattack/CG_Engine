#ifndef CG_ENGINE_ENGINE_H
#define CG_ENGINE_ENGINE_H

#include "CG_Engine/ui/gui.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const sf::ContextSettings settings{24, 8, 0, 3, 3, sf::ContextSettings::Default};


class Engine {
private:
    Engine() : isRun(true), isGame(false), viewSize(0) {
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
    };

    bool isRun;
    bool isGame;
    sf::RenderWindow window;

    Vec2 viewSize;

    GUI gui;

public:
    ~Engine() {
        window.close();
    };

    Engine(const Engine &) = delete;

    Engine &operator=(const Engine &) = delete;

    static Engine &Ref() {
        static Engine reference;
        return reference;
    }

    void exit() {
        isRun = false;
        window.close();
    }

    void update() {
        Manager.update();
        Manager.render();
        gui.display(window);
        glEnable(GL_DEPTH_TEST);

        window.display();
    };

    sf::Clock deltaClock;

    void render() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!isGame) {

        } else {
            Manager.render();
        }
    };

    void startGame() {
        Manager.deactivateEditorSystems();
        Manager.activateRuntimeSystems();
        isGame = true;
    };

    void stopGame() {
        isGame = false;
        Manager.deactivateRuntimeSystems();
        Manager.activateEditorSystems();
    };

    void init() {
        // register component list
        Manager.registerCompList<Camera>();
        Manager.registerCompList<Transform>();
        Manager.registerCompList<SpotLighting>();
        Manager.registerCompList<EntityName>();
        Manager.registerCompList<PointLighting>();
        Manager.registerCompList<MeshRenderer>();
        Manager.registerCompList<ModelRenderer>();
//        Manager.registerCompList<SpriteRenderer>();
        Manager.registerCompList<DirectionalLight>();
        Manager.registerCompList<KeyMotion>();

        // register component factory
        Registrar<Camera>("Camera");
        Registrar<SpotLighting>("Spot Light");
        Registrar<SpotLighting>("Point Light");
        Registrar<MeshRenderer>("MeshRenderer");
        Registrar<ModelRenderer>("ModelRenderer");
//        Registrar<SpriteRenderer>("SpriteRenderer");
        Registrar<DirectionalLight>("Directional Light");
        Registrar<EntityName>("EntityName");
        Registrar<KeyMotion>("KeyMotion");

        // register systems
        Manager.addSystem<SpotLightSystem>();
        Manager.addSystem<PointLightSystem>();
        Manager.addSystem<DirectionalLightSystem>();
        Manager.addSystem<KeyMotionSystem>();

        Manager.addSystem<MeshRendererSystem>();
        Manager.addSystem<ModelRendererSystem>();
//        Manager.addSystem<SpriteRendererSystem>();

        // runtime systems will be added at runtime
        Manager.addSystem<CameraSystem>();
//        std::cout << "im here";

        Manager.activateEditorSystems();
        Manager.start();

        stopGame();

        gui.init();
    };

    Vec2 ViewSize() const {
        return viewSize;
    }

    sf::RenderWindow &Window() {
        return window;
    }

    bool run() const {
        return isRun;
    }
};

static Engine &core = Engine::Ref();

#endif
