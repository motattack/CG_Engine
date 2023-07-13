#ifndef CG_ENGINE_TIMER_H
#define CG_ENGINE_TIMER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Timer {
private:
    float lastFrame, dt;

    Timer() : dt(0.f), lastFrame(0.f) {};

    sf::Clock clock;

public:
    ~Timer() {

    }

    Timer &operator=(const Timer &) = delete;

    static Timer &Ref() {
        static Timer reference;
        return reference;
    }

    void init() {

    };

    void tick() {
        dt = clock.getElapsedTime().asSeconds() - lastFrame;
        lastFrame = clock.getElapsedTime().asSeconds();
    };

    float deltaTime() {
        return dt;
    }
};

static Timer &timer = Timer::Ref();

#endif //CG_ENGINE_TIMER_H
