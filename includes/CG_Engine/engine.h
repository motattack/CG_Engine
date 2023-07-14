#ifndef CG_ENGINE_ENGINE_H
#define CG_ENGINE_ENGINE_H

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <GL/glew.h>
#include <CG_Engine/timer.h>
#include <CG_Engine/base/entityManager.h>
#include <CG_Engine/math/common.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const sf::ContextSettings settings{24, 8, 0, 3, 3, sf::ContextSettings::Default};
enum class Mode {
    EDIT = 0,
    GAME = 1
};


class Engine {
private:
    bool isRun;
    int width, height;
    sf::RenderWindow window;

    Engine();

    void initEntity();

public:
    float zoom;
    Mat4x4 view;
    Mat4x4 projection;
    Vec3 mainCameraPosition;
    Mode mode;
    EntityManager Manager;

    Engine(const Engine &) = delete;

    Engine &operator=(const Engine &) = delete;

    ~Engine() {
        window.close();
    };

    static Engine &Ref() {
        static Engine reference;
        return reference;
    };

    void init();


    void update();

    void exit() {
        isRun = false;
    };

    bool run() const {
        return isRun;
    }

    sf::RenderWindow &getWindow() {
        return window;
    }

    float videoWidth() {
        return width;
    }

    float videoHeight() {
        return height;
    }
};

static Engine &core = Engine::Ref();
static EntityManager &Manager = core.Manager;

static float &zoom = core.zoom;
static Mat4x4 &view = core.view;
static Mat4x4 &projection = core.projection;
static Vec3 &mainCameraPosition = core.mainCameraPosition;

#endif
