#ifndef CG_ENGINE_EVENTS_H
#define CG_ENGINE_EVENTS_H

#include <imgui.h>
#include <imgui-SFML.h>
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
            ImGui::SFML::ProcessEvent(core.getWindow(), event);
            if (event.type == sf::Event::Closed)
                core.exit();
            if (event.type == sf::Event::MouseButtonPressed)
                MouseButtonCallback(event.mouseButton.button, true);
            if (event.type == sf::Event::MouseButtonReleased)
                MouseButtonCallback(event.mouseButton.button, false);
            if (event.type == sf::Event::KeyPressed)
                KeyboardCallback(event.key.code, true);
            if (event.type == sf::Event::KeyReleased)
                KeyboardCallback(event.key.code, false);
            if (event.type == sf::Event::Resized) {
                sf::Vector2u size = core.getWindow().getSize();
                WindowResizedCallback(size.x, size.y);
            }
            if (event.type == sf::Event::Closed) {
                window_close_callback(core.getWindow());
            }
        }
    };

    void init() {
    };

};

static Events &event = Events::Ref();

#endif //CG_ENGINE_EVENTS_H