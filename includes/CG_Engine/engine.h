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

const int SHADOW_WIDTH = 1024;
const int SHADOW_HEIGHT = 1024;

const sf::ContextSettings settings{24, 8, 0, 3, 3, sf::ContextSettings::Default};
enum class Mode {
    EDIT = 0,
    GAME = 1
};


class Engine {
private:
    Engine();

    bool isRun;
    bool isGame;
    sf::RenderWindow window;

    Vec2 viewSize;

public:
    ~Engine();

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

    void update();

    void render();

    void startGame();

    void stopGame();

    void init();

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
