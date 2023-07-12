#ifndef CG_ENGINE_EVENTS_H
#define CG_ENGINE_EVENTS_H

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <CG_Engine/engine.h>

class Events {
private:
    Events() {};

public:
    ~Events() {};

    Events(const Events &) = delete;

    Events &operator=(const Events &) = delete;

    static Events &Ref() {
        static Events reference;
        return reference;
    }

    void poll() {
        sf::Event event{};
        while (core.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                core.exit();
        }
    };

    void init() {
    };

};

static Events &event = Events::Ref();

#endif //CG_ENGINE_EVENTS_H