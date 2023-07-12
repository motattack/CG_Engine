#ifndef CG_ENGINE_ENGINE_H
#define CG_ENGINE_ENGINE_H

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include "GL/glew.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const sf::ContextSettings settings{24, 8, 0, 3, 3, sf::ContextSettings::Default};


class Engine {
private:
    bool isRun;
    float width, height;
    sf::RenderWindow window;

    Engine() : isRun(true), width(SCREEN_WIDTH), height(SCREEN_HEIGHT) {
        window.create(sf::VideoMode(width, height), "CG_Engine", sf::Style::Titlebar | sf::Style::Close,
                      settings);
        window.setFramerateLimit(75);
        window.setVerticalSyncEnabled(true);
        window.setActive(true);

        glewExperimental = GL_TRUE;

        if (GLEW_OK != glewInit()) {
            throw std::runtime_error("Error:: glew not init =(");
        }

        glEnable(GL_DEPTH_TEST);
    };
public:
    Engine(const Engine &) = delete;

    Engine &operator=(const Engine &) = delete;

    ~Engine() {
        window.close();
    };

    static Engine &Ref() {
        static Engine reference;
        return reference;
    };

    void init() {

    };

    void update() {

    };

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

#endif
